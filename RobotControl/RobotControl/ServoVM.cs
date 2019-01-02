using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Addle.Wpf.ViewModel;

namespace RobotControl
{
    public class ServoVM : ViewModelBase
    {
        Action<float> _valueChanged;

        public ServoVM(string name, Action<float> valueChanged)
        {
            Name = name;
            _valueChanged = valueChanged;
        }

        public string Name { get; private set; }
        public int ServoIndex { get; private set; }

        #region float Min (VM property)
        float _min = 0;
        public float Min { get { return _min; } set { SetProperty(ref _min, value); } }
        #endregion

        #region float Max (VM property)
        float _max = 180;
        public float Max { get { return _max; } set { SetProperty(ref _max, value); } }
        #endregion

        #region float Value (VM property w/ OnChanged)
        float _value;
        public float Value { get { return _value; } set { SetProperty(ref _value, value, v => ValueChanged(v)); } }
        #endregion

        void ValueChanged(float v)
        {
            _valueChanged(v);
        }
    }
}
