#pragma once

#include <memory>

class SimpleTimer {
public:
    SimpleTimer(const char *name, int sampleCount);

    static void SetEnabled(bool value) { _enabled = value; }

    void Start();
    void Stop();

private:
    static bool _enabled;

    std::string _name;
    long _sampleCount = 0;
    long _count = 0;
    long _startMicros = 0;
    uint64_t _sum = 0;
};
