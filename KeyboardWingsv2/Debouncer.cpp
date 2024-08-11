#include "Debouncer.h"

#include <Arduino.h>

void Debouncer::setDebounceTime(int debounceMicros)
{
    _debounceMicros = debounceMicros;
}

void Debouncer::setValue(bool v, bool force) {
    long now = micros();
    if (force || now - _lastChangedTime >= _debounceMicros) {
        _current = v;
        _lastChangedTime = now;
    }
}
