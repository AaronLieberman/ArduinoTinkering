#pragma once

#include "stlhelper.h"
#include <Arduino.h>

template <typename T>
class Debouncer {
public:
    Debouncer(int debounceMicros, T initial)
        : _debounceMicros(debounceMicros)
        , _current(initial) { }

    void setValue(T v, bool force = false) {
        uint now = micros();
        if (force || now - _lastChangedTime >= _debounceMicros) {
            _current = v;
            _lastChangedTime = now;
        }
    }

    T getValue() const { return _current; }

private:
    const int _debounceMicros;
    T _current;
    uint _lastChangedTime = 0;
};
