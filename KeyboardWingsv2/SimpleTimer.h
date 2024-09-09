#pragma once

#include <memory>

class SimpleTimer {
public:
    SimpleTimer(const char *name, int sampleCount, bool enabled = true);

    void Start();
    void Stop();

private:
    std::string _name;
    long _sampleCount = 0;
    bool _enabled = false;
    long _count = 0;
    long _startMicros = 0;
    uint64_t _sum = 0;
};
