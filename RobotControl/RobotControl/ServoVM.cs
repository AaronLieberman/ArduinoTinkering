using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Linq;
using System.Reactive.Subjects;
using System.Text;
using System.Threading.Tasks;
using Addle.Wpf.ViewModel;

namespace RobotControl
{
    public class ServoVM : ViewModelBase
    {
        readonly Subject<float> _valueChangedThrottled = new Subject<float>();

        public ServoVM(string name, Action<float> valueChanged, float min, float max, float initialValue)
        {
            Name = name;
            Min = min;
            Max = max;
            Value = initialValue;

            _valueChangedThrottled.Throttle(TimeSpan.FromMilliseconds(10)).Subscribe(valueChanged);
        }

        public string Name { get; }

        #region float Min (VM property)
        float _min = 150;
        public float Min
        {
            get => _min;
            set => SetProperty(ref _min, value);
        }
        #endregion

        #region float Max (VM property)
        float _max = 500;
        public float Max
        {
            get => _max;
            set => SetProperty(ref _max, value);
        }
        #endregion

        #region float Value (VM property w/ OnChanged)
        float _value;
        public float Value
        {
            get => _value;
            set => SetProperty(ref _value, value, ValueChanged);
        }
        #endregion

        void ValueChanged(float v)
        {
            _valueChangedThrottled.OnNext(v);
        }
    }
}
