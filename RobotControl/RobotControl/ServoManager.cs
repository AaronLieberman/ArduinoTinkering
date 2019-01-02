using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Reactive.Subjects;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Addle.Core.Linq;

namespace RobotControl
{
    class ServoManager : IDisposable
    {
        readonly List<Servo> _servos = new List<Servo>();
        readonly Dictionary<int, float> _lastSentValues = new Dictionary<int, float>();
        readonly Dictionary<int, float> _currentValues = new Dictionary<int, float>();
        readonly Subject<string> _messageReceived = new Subject<string>();
        bool _running = false;
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

        public IEnumerable<Servo> Servos { get { return _servos; } }
        public IObservable<string> MessageReceived { get { return _messageReceived; } }

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
            using (var serialPort = new SerialPort(serialPortName, 115200))
            {
                serialPort.Open();
                var buffer = new byte[4096];

                while (_running)
                {
                    if (serialPort.BytesToRead > 0)
                    {
                        var count = serialPort.Read(buffer, 0, buffer.Length);

                        // throw away read bytes for now
                    }
                    else
                    {
                        Thread.Yield();
                    }

                    Debug.Assert(_currentValues.Count == _lastSentValues.Count);
                    for (int i = 0; i < _currentValues.Count; i++)
                    {
                        serialPort.WriteLine($"{i}: {_currentValues[i]}");
                    }
                }

                serialPort.Close();
            }
        }
    }
}
