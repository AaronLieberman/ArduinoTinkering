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
        readonly ServoManager _servoManager;
        readonly ObservableCollection<string> _receivedMessages = new ObservableCollection<string>();

        public RobotVM(ServoManager servoManager)
        {
            _servoManager = servoManager;

            foreach (var servo in servoManager.Servos)
            {
                _servos.Add(new ServoVM(servo.Name, v => servoManager.SetServoPosition(servo.ServoIndex, v)));
            }

            ConnectCommand = new ActionCommand(() => _servoManager.Start(_serialPortName));
            ClearMessagesCommand = new ActionCommand(() => _receivedMessages.Clear());

            _servoManager.MessageReceived.ObserveOnDispatcher().Subscribe(s => MessageReceived(s));
        }

        public IEnumerable<ServoVM> Servos { get { return _servos; } }
        public IEnumerable<string> ReceivedMessages { get { return _receivedMessages; } }
        
        #region string SerialPortName (VM property)
        string _serialPortName = "COM6";
        public string SerialPortName { get { return _serialPortName; } set { SetProperty(ref _serialPortName, value); } }
        #endregion

        #region string ReceivedMessageIndex (VM property)
        int _receivedMessageIndex = -1;
        public int ReceivedMessageIndex { get { return _receivedMessageIndex; } set { SetProperty(ref _receivedMessageIndex, value); } }
        #endregion

        public ActionCommand ConnectCommand { get; }
        public ActionCommand ClearMessagesCommand { get; }

        void MessageReceived(string message)
        {
            _receivedMessages.Add(message);
            _receivedMessageIndex = _receivedMessages.Count - 1;
        }

        void ClearMessages(string message)
        {
            _receivedMessages.Clear();
            _receivedMessageIndex = -1;
        }
    }
}
