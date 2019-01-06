using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reactive.Subjects;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Addle.Core.Threading;

namespace RobotControl
{
    class ServoManager : IDisposable
    {
        class Synchronized
        {
            public Synchronized()
            {
                CurrentOutputEnabled = true;
            }

            public Dictionary<int, float> LastSentValues { get; } = new Dictionary<int, float>();
            public Dictionary<int, float> CurrentValues { get; } = new Dictionary<int, float>();
            public bool LastSentOutputEnabled { get; set; }
            public bool CurrentOutputEnabled { get; set; }
        }

        readonly List<Servo> _servos = new List<Servo>();
        readonly SynchronizedValue<Synchronized> _state = new SynchronizedValue<Synchronized>(new Synchronized());
        readonly Subject<string> _messageReceived = new Subject<string>();
        bool _running;
        Task _work;

        public ServoManager()
        {
            var servos = new[]
            {
                new {Name = "R Lower", Min = 150, Max = 500, Initial = 330},
                new {Name = "R Upper", Min = 150, Max = 500, Initial = 320},
                new {Name = "R Shoulder", Min = 150, Max = 500, Initial = 345},
                new {Name = "R Torso", Min = 150, Max = 500, Initial = 290},
                new {Name = "L Lower", Min = 150, Max = 500, Initial = 330},
                new {Name = "L Upper", Min = 150, Max = 500, Initial = 320},
                new {Name = "L Shoulder", Min = 150, Max = 500, Initial = 345},
                new {Name = "L Torso", Min = 150, Max = 500, Initial = 290}
            };

            using (var state = _state.Lock())
            {
                foreach (var element in servos.Select((a, i) => new {Servo = a, Index = i}))
                {
                    var servo = element.Servo;
                    _servos.Add(new Servo(servo.Name, element.Index, servo.Min, servo.Max, servo.Initial));
                    state.Value.CurrentValues[element.Index] = servo.Initial;
                    state.Value.LastSentValues[element.Index] = float.MinValue;
                }
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
            using (var currentValues = _state.Lock())
            {
                currentValues.Value.CurrentValues[index] = value;
            }
        }

        public bool OutputEnabled
        {
            get => _state.WithLock(v => v.CurrentOutputEnabled);
            set => _state.WithLock(v => v.CurrentOutputEnabled = value);
        }

        void Run(string serialPortName)
        {
            using (var serialPort = new SerialPortCommunicator(serialPortName))
            {
                serialPort.Open();
                var readBuffer = new byte[4096];
                int readBufferPos = 0;

                Thread.Sleep(1000);

                void SendText(string s)
                {
                    var bytesToWrite = Encoding.ASCII.GetBytes(s);
                    serialPort.Write(bytesToWrite, 0, bytesToWrite.Length);
                }

                while (_running)
                {
                    var bytesRead = serialPort.Read(readBuffer, readBufferPos, readBuffer.Length - readBufferPos);
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
                                _messageReceived.OnNext(line);
                                break;
                            }
                        }
                    }
                    else
                    {
                        Thread.Yield();
                    }

                    using (var state = _state.Lock())
                    {
                        var value = state.Value;
                        Debug.Assert(value.CurrentValues.Count == value.LastSentValues.Count);
                        for (int i = 0; i < value.CurrentValues.Count; i++)
                        {
                            if (Math.Abs(value.LastSentValues[i] - value.CurrentValues[i]) > 0.001f)
                            {
                                var stringToSend = $"servo {i}: {value.CurrentValues[i]}\r\n";
                                SendText(stringToSend);
                                value.LastSentValues[i] = value.CurrentValues[i];
                            }
                        }

                        if (value.LastSentOutputEnabled != value.CurrentOutputEnabled)
                        {
                            var stringToSend = $"outputEnable: {(value.CurrentOutputEnabled ? "true" : "false")}\r\n";
                            SendText(stringToSend);
                            value.LastSentOutputEnabled = value.CurrentOutputEnabled;
                        }
                    }
                }

                SendText("outputEnable: false\r\n");

                serialPort.Close();
            }
        }
    }
}
