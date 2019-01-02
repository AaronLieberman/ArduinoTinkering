using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Addle.Wpf;
using Addle.Wpf.ViewModel;

namespace RobotControl
{
    class RobotVM : ViewModelBase
    {
        readonly List<ServoVM> _servos = new List<ServoVM>();
        readonly ServoManager _servoManager;
        readonly ActionCommand _connectCommand;

        public RobotVM(ServoManager servoManager)
        {
            _servoManager = servoManager;
            var servos = servoManager.Servos;
            foreach (var servo in servos)
            {
                _servos.Add(new ServoVM(servo.Name, v => servoManager.SetServoPosition(servo.ServoIndex, v)));
            }

            _connectCommand = new ActionCommand(() => _servoManager.Start(_serialPortName));
        }

        public IEnumerable<ServoVM> Servos { get { return _servos; } }

        #region string SerialPortName (VM property)
        string _serialPortName = "COM6";
        public string SerialPortName { get { return _serialPortName; } set { SetProperty(ref _serialPortName, value); } }
        #endregion

        public ActionCommand Connect { get { return _connectCommand; } }
    }
}
