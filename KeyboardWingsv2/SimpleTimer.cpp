#include "SimpleTimer.h"

#include "SerialPrintf.h"

#include <Arduino.h>

SimpleTimer::SimpleTimer(int maxCount)
    : _maxCount(maxCount) {
}

void SimpleTimer::Start() {
    _startMicros = micros();
}

void SimpleTimer::Stop() {
    long stopMicros = micros();
    _sum += stopMicros - _startMicros;
    if (((++_count) % _maxCount) == 0) {
        serialPrintfln("%d", (long)(_sum / _maxCount));
        _sum = 0;
    }
}
