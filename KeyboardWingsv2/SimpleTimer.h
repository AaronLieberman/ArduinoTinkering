#pragma once

#include <memory>

class SimpleTimer {
public:
    SimpleTimer(int maxCount);

    void Start();
    void Stop();

private:
    long _maxCount = 0;
    long _count = 0;
    long _startMicros = 0;
    uint64_t _sum = 0;
};
