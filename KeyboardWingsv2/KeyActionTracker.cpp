#include "KeyActionTracker.h"

#include <array>

LayoutKey KeyActionTracker::KeyDown(LayoutKey key, int count) {
    LayoutKey result = {};
    unsigned long now = millis();  // Get the current time in milliseconds

    // check if this key has been pressed before
    if (_keyData.count(key) == 0) {
        // first time press, initialize the data for this key
        _keyData[key] = {now, 1};
    } else {
        auto& [start_time, press_count] = _keyData[key];

        // calculate the time since the first press in the sequence
        unsigned long duration = now - start_time;

        if (duration <= kSuccessionDuration) {
            // within the time window, increment the count
            press_count++;

            if (press_count == count) {
                result = key;
                _keyData[key] = {now, 0};
            }
        } else {
            // outside the time window, reset the timer and count
            _keyData[key] = {now, 1};
        }
    }

    return result;
}
