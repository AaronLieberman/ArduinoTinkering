using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotControl
{
    class Servo
    {
        public Servo(string name, int servoIndex)
        {
            Name = name;
            ServoIndex = servoIndex;
        }

        public string Name { get; private set; }
        public int ServoIndex { get; private set; }
    }
}
