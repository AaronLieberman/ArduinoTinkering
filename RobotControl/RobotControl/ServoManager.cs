using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reactive.Subjects;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RobotControl
{
    class ServoManager : IDisposable
    {
        readonly List<Servo> _servos = new List<Servo>();
        readonly Dictionary<int, float> _lastSentValues = new Dictionary<int, float>();
        readonly Dictionary<int, float> _currentValues = new Dictionary<int, float>();
        readonly Subject<string> _messageReceived = new Subject<string>();
        bool _running;
        Task _work;

        public ServoManager()
        {
            var servos = new[]
            {
                new { Name = "R Torso" },
                new { Name = "R Shoulder" },
                new { Name = "R Upper" },
                new { Name = "R Lower" },
                new { Name = "L Torso" },
                new { Name = "L Shoulder" },
                new { Name = "L Upper" },
                new { Name = "L Lower" }
            };

            foreach (var element in servos.Select((a, i) => new { Servo = a, Index = i }))
            {
                _servos.Add(new Servo(element.Servo.Name, element.Index));
                _currentValues[element.Index] = 0;
                _lastSentValues[element.Index] = float.MinValue;
            }
        }

        public void Dispose()
        {
            Stop();
        }

        public IEnumerable<Servo> Servos => _servos;
        public IObservable<string> MessageReceived => _messageReceived;

        public void Start(string serialPortName)
        {
            Stop();

            _running = true;
            _work = Task.Run(() => Run(serialPortName));
        }

        public void Stop()
        {
            if (_work != null)
            {
                _running = false;
                _work.Wait();
            }
        }

        public void SetServoPosition(int index, float value)
        {
            _currentValues[index] = value;
        }

        void Run(string serialPortName)
        {
            using (var serialPort = new SerialPortCommunicator(serialPortName))
            {
                serialPort.Open();
                var readBuffer = new byte[4096];
                int readBufferPos = 0;
                //var writeBuffer = new byte[serialPort.WriteBufferSize];

                while (_running)
                {
                    var bytesRead = serialPort.Read(readBuffer, readBufferPos,
                        readBuffer.Length - readBufferPos);
                    readBufferPos += bytesRead;

                    if (bytesRead > 0)
                    {
                        for (var i = 0; i < readBufferPos; i++)
                        {
                            if (readBuffer[i] == '\n')
                            {
                                var line = Encoding.ASCII.GetString(readBuffer, 0, i).Trim();
                                Buffer.BlockCopy(readBuffer, i + 1, readBuffer, 0, readBufferPos - (i + 1));
                                readBufferPos -= i + 1;
                                Console.WriteLine(line);
                                break;
                            }
                        }
                    }
                    else
                    {
                        Thread.Yield();
                    }

                    Debug.Assert(_currentValues.Count == _lastSentValues.Count);
                    for (int i = 0; i < _currentValues.Count; i++)
                    {
                        //serialPort.WriteLine($"{i}: {_currentValues[i]}");
                    }
                }

                serialPort.Close();
            }
        }
    }
}
