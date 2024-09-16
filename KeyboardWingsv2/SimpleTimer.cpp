#include "SimpleTimer.h"

#include "SerialPrintf.h"

#include <Arduino.h>

bool SimpleTimer::_enabled = true;

SimpleTimer::SimpleTimer(const char *name, int sampleCount)
    : _name(name)
    , _sampleCount(sampleCount) {
}

void SimpleTimer::Start() {
    _startMicros = micros();
}

void SimpleTimer::Stop() {
    if (!_enabled) {
        return;
    }

    long stopMicros = micros();
    _sum += stopMicros - _startMicros;
    if (((++_count) % _sampleCount) == 0) {
        serialPrintfln("timer %s: %d", _name.c_str(), (long)(_sum / _sampleCount));
        _sum = 0;
    }
}
