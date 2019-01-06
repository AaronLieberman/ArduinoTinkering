using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotControl
{
    class Servo
    {
        public Servo(string name, int servoIndex, float min, float max, float initialValue)
        {
            Name = name;
            ServoIndex = servoIndex;
            Min = min;
            Max = max;
            InitialValue = initialValue;
        }

        public string Name { get; }
        public int ServoIndex { get; }
        public float Min { get; }
        public float Max { get; }
        public float InitialValue { get; }
    }
}
