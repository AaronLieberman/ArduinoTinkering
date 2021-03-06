﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

namespace RobotControl
{
    class SerialPortCommunicator : IDisposable
    {
        #region P/Invoke

        static class SerialPortExternals
        {
            [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
            public static extern IntPtr CreateFile(
                [In] [MarshalAs(UnmanagedType.LPTStr)] string filename,
                [In] [MarshalAs(UnmanagedType.U4)] FileAccess access,
                [In] [MarshalAs(UnmanagedType.U4)] FileShare share,
                [In] IntPtr securityAttributes, // optional SECURITY_ATTRIBUTES struct or IntPtr.Zero
                [In] [MarshalAs(UnmanagedType.U4)] FileMode creationDisposition,
                [In] [MarshalAs(UnmanagedType.U4)] FileAttributes flagsAndAttributes,
                [In] IntPtr templateFile);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool CloseHandle([In] IntPtr hFile);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool ReadFile([In] IntPtr hFile, [In] [Out] byte[] lpBuffer,
                [In] uint nNumberOfBytesToRead, [Out] out uint lpNumberOfBytesRead, [In] IntPtr lpOverlapped);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool WriteFile([In] IntPtr hFile, [In] [Out] byte[] lpBuffer,
                [In] uint nNumberOfBytesToWrite, [Out] out uint lpNumberOfBytesWritten, [In] IntPtr lpOverlapped);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool SetCommState([In] IntPtr hFile, [In] byte[] lpDcb);

            [DllImport("kernel32.dll", SetLastError = true)]
            public static extern bool SetCommTimeouts([In] IntPtr hFile, [In] byte[] lpCommTimeouts);
        }

        #endregion

        readonly object _lockObject = new object();
        Task _task;
        CancellationTokenSource _cancellation;
        MemoryStream _readStream;
        MemoryStream _writeStream;
        readonly string _serialPortName;

        public SerialPortCommunicator(string serialPortName)
        {
            _serialPortName = serialPortName;
        }

        public void Dispose()
        {
            Close();
        }

        public void Open()
        {
            lock (_lockObject)
            {
                Debug.Assert(_task == null);
                _cancellation = new CancellationTokenSource();
                _readStream = new MemoryStream();
                _writeStream = new MemoryStream();
                _task = Task.Run(() => ReadIntoCollection(_readStream, _writeStream, _cancellation.Token));
            }
        }

        public void Close()
        {
            Task task = _task;

            lock (_lockObject)
            {
                if (_task != null)
                {
                    _cancellation.Cancel();
                    _cancellation = null;
                    _readStream = null;
                    _task = null;
                }
            }

            task?.Wait();
        }

        public int Read(byte[] buffer, int offset, int count)
        {
            lock (_lockObject)
            {
                if (_readStream.Length == 0) return 0;

                var bytes = _readStream.GetBuffer();
                int bytesCopied = Math.Min(count, (int)_readStream.Length);
                Buffer.BlockCopy(bytes, 0, buffer, offset, bytesCopied);

                if (bytesCopied == _readStream.Length)
                {
                    _readStream.SetLength(0);
                }
                else
                {
                    Buffer.BlockCopy(bytes, bytesCopied, bytes, 0, (int)_readStream.Length - bytesCopied);
                    _readStream.SetLength(_readStream.Length - bytesCopied);
                }

                return bytesCopied;
            }
        }

        public void Write(byte[] buffer, int offset, int count)
        {
            if (count == 0) return;

            lock (_lockObject)
            {
                _writeStream.Write(buffer, offset, count);
            }
        }

        void ReadIntoCollection(Stream readStream, MemoryStream writeStream, CancellationToken cancellation)
        {
            IntPtr hComm = IntPtr.Zero;
            try
            {
                hComm = SerialPortExternals.CreateFile($"\\\\?\\{_serialPortName}", FileAccess.ReadWrite,
                    FileShare.None, IntPtr.Zero, FileMode.Open, 0, IntPtr.Zero);
                Debug.Assert(hComm != IntPtr.Zero);

                // magic bytes that represents BuildCommDCB(L"115200,n,8,1", &dcb)
                var dcbBytes = new byte[] { 28, 0, 0, 0, 0, 194, 1, 0, 16, 16, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                var setComStateResult = SerialPortExternals.SetCommState(hComm, dcbBytes);
                Debug.Assert(setComStateResult);

                // magic bytes that represents COMMTIMEOUTS timeouts = {}; timeouts.ReadIntervalTimeout = MAXDWORD;
                var comTimeoutsBytes = new byte[] { 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                var setComTimeoutsResult = SerialPortExternals.SetCommTimeouts(hComm, comTimeoutsBytes);
                Debug.Assert(setComTimeoutsResult);

                var buffer = new byte[4096];
                while (!cancellation.IsCancellationRequested)
                {
                    var readFileResult = SerialPortExternals.ReadFile(hComm, buffer, (uint)buffer.Length, out var bytesRead, IntPtr.Zero);
                    Debug.Assert(readFileResult);

                    bool anyWorkDone = false;

                    if (bytesRead > 0)
                    {
                        anyWorkDone = true;
                        lock (_lockObject)
                        {
                            readStream.Write(buffer, 0, (int)bytesRead);
                        }
                    }

                    bool anyBytesToWrite = false;
                    lock (_lockObject)
                    {
                        anyBytesToWrite = writeStream.Length > 0;
                    }

                    if (anyBytesToWrite)
                    {
                        var attemptToWriteBytes = Math.Min((int)writeStream.Length, buffer.Length);
                        lock (_lockObject)
                        {
                            Buffer.BlockCopy(writeStream.GetBuffer(), 0, buffer, 0, attemptToWriteBytes);
                        }

                        var writeFileResult = SerialPortExternals.WriteFile(hComm, buffer, (uint)attemptToWriteBytes,
                            out var bytesWritten, IntPtr.Zero);

                        if (writeFileResult && bytesWritten > 0)
                        {
                            lock (_lockObject)
                            {
                                var bytesRemaining = (int)(writeStream.Length - bytesWritten);
                                if (bytesRemaining > 0)
                                {
                                    Buffer.BlockCopy(writeStream.GetBuffer(), (int)bytesWritten,
                                        writeStream.GetBuffer(), 0, bytesRemaining);
                                }

                                writeStream.SetLength(bytesRemaining);
                            }
                        }

                        anyWorkDone = true;
                    }

                    if (!anyWorkDone)
                    {
                        Thread.Yield();
                    }
                }
            }
            finally
            {
                if (hComm != IntPtr.Zero)
                {
                    SerialPortExternals.CloseHandle(hComm);
                }
            }
        }
    }

    #region NewLineStream?

    class NewLineStream : Stream
    {
        readonly object _lockObject = new object();
        readonly MemoryStream _stream = new MemoryStream();
        int _foundNewlinePos = -1;

        public void AddData(byte[] buffer, int count)
        {
            if (count == 0) return;

            lock (_lockObject)
            {
                _stream.Write(buffer, 0, count);
                if (_foundNewlinePos == -1)
                {
                    var streamBuffer = _stream.GetBuffer();
                    for (int i = 0; i < streamBuffer.Length; i++)
                    {
                        if (streamBuffer[i] == '\n')
                        {
                            _foundNewlinePos = i;
                            break;
                        }
                    }
                }
            }
        }

        public override void Flush()
        {
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            throw new NotSupportedException();
        }

        public override void SetLength(long value)
        {
            throw new NotSupportedException();
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            int bytesRead = 0;

            lock (_lockObject)
            {
                if (_foundNewlinePos >= 0)
                {
                    bytesRead = Math.Min(count, _foundNewlinePos);
                    Buffer.BlockCopy(_stream.GetBuffer(), 0, buffer, offset, bytesRead);
                    _stream.SetLength(_stream.GetBuffer().Length - bytesRead);
                }
            }

            return bytesRead;
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            throw new NotSupportedException();
        }

        public override bool CanRead => true;
        public override bool CanSeek => false;
        public override bool CanWrite => false;

        public override long Length => throw new NotSupportedException();

        public override long Position
        {
            get => Math.Max(_foundNewlinePos, 0);
            set => throw new NotSupportedException();
        }
    }

    #endregion
}
