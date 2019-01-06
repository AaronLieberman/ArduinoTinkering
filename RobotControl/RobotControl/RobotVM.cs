using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reactive.Linq;
using Addle.Wpf;
using Addle.Wpf.ViewModel;

namespace RobotControl
{
    class RobotVM : ViewModelBase
    {
        readonly List<ServoVM> _servos = new List<ServoVM>();
        readonly ObservableCollection<string> _receivedMessages = new ObservableCollection<string>();
        readonly ServoManager _servoManager;

        public RobotVM(ServoManager servoManager)
        {
            _servoManager = servoManager;

            foreach (var servo in servoManager.Servos)
            {
                // copy loop variable so we're getting each servo separately
                var servoCopy = servo;
                _servos.Add(new ServoVM(servo.Name, v => SetServoPosition(servoCopy, v), servo.Min, servo.Max, servo.InitialValue));
            }

            OutputEnabled = _servoManager.OutputEnabled;

            ConnectCommand = new ActionCommand(() => servoManager.Start(_serialPortName));
            ClearMessagesCommand = new ActionCommand(ClearMessages);

            servoManager.MessageReceived.ObserveOnDispatcher().Subscribe(MessageReceived);
        }

        public IEnumerable<ServoVM> Servos => _servos;
        public IEnumerable<string> ReceivedMessages => _receivedMessages;

        #region string SerialPortName (VM property)
        string _serialPortName = "COM6";
        public string SerialPortName
        {
            get => _serialPortName;
            set => SetProperty(ref _serialPortName, value);
        }
        #endregion

        #region string ReceivedMessageIndex (VM property)
        int _receivedMessageIndex = -1;
        public int ReceivedMessageIndex
        {
            get => _receivedMessageIndex;
            set => SetProperty(ref _receivedMessageIndex, value);
        }
        #endregion

        #region bool OutputEnabled (VM property w/ changed)
        bool _outputEnabled = true;
        public bool OutputEnabled
        {
            get => _outputEnabled;
            set => SetProperty(ref _outputEnabled, value, OutputEnabledChanged);
        }

        #endregion

        public ActionCommand ConnectCommand { get; }
        public ActionCommand ClearMessagesCommand { get; }

        void MessageReceived(string message)
        {
            _receivedMessages.Insert(0, message);
            _receivedMessageIndex = _receivedMessages.Count - 1;
        }

        void ClearMessages()
        {
            _receivedMessages.Clear();
            _receivedMessageIndex = -1;
        }

        void SetServoPosition(Servo servo, float v)
        {
            _servoManager.SetServoPosition(servo.ServoIndex, v);
        }

        void OutputEnabledChanged(bool value)
        {
            _servoManager.OutputEnabled = value;
        }
    }
}
