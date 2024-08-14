#pragma once

#include <memory>

class Debouncer {
public:
    static void setDebounceTime(long debounceMs);

    bool setValue(bool v, bool force = false);
    bool getValue() const { return _current; }

private:
    static long kDebounceMs;

    long _requestTime = 0;
    bool _current = false;
    bool _request = false;
};
