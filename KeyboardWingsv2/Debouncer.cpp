#include "Debouncer.h"

#include <Arduino.h>

long Debouncer::kDebounceMs = -1;

void Debouncer::setDebounceTime(long debounceMs) {
    kDebounceMs = debounceMs;
}

bool Debouncer::setValue(bool v, bool force) {
    long now = millis();

    // must setDebounceTime first
    if (Debouncer::kDebounceMs < 0) {
        return false;
    }

    if (force || Debouncer::kDebounceMs == 0) {
        _request = v;
        _current = v;
        _requestTime = now;
        return _current;
    }

    if (v != _request) {
        _request = v;
        _requestTime = now;
    } else if (now >= _requestTime + kDebounceMs) {
        _current = _request;
    }

    return _current;
}
