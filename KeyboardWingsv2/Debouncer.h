#pragma once

#include <memory>

class Debouncer {
public:
    void setDebounceTime(int debounceMicros);

    void setValue(bool v, bool force = false);
    bool getValue() const { return _current; }

private:
    int _debounceMicros = 2000;
    bool _current;
    long _lastChangedTime = 0;
};
