#include "SimpleTimer.h"

#include "SerialPrintf.h"

#include <Arduino.h>

SimpleTimer::SimpleTimer(int sampleCount)
    : _sampleCount(sampleCount) {
}

void SimpleTimer::Start() {
    _startMicros = micros();
}

void SimpleTimer::Stop() {
    long stopMicros = micros();
    _sum += stopMicros - _startMicros;
    if (((++_count) % _sampleCount) == 0) {
        serialPrintfln("%d", (long)(_sum / _sampleCount));
        _sum = 0;
    }
}
