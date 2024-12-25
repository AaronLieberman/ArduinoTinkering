#include "KeyActionTracker.h"

#include <array>

void KeyActionTracker::KeyDown(LayoutKey key) {
    LayoutKey result = {};
    unsigned long now = millis();  // Get the current time in milliseconds

    // check if this key has been pressed before
    if (_keyData.count(key) == 0) {
        // first time press, initialize the data for this key
        _keyData[key] = {now, 1, true};
    } else {
        auto& [startTime, pressCount, currentlyDown] = _keyData[key];

        currentlyDown = true;

        // calculate the time since the first press in the sequence
        unsigned long duration = now - startTime;

        if (duration <= kRepeatWindowMs) {
            // within the time window, increment the count
            pressCount++;
        } else {
            // outside the time window, reset the timer and count
            startTime = now;
            pressCount = 1;
        }
    }
}

void KeyActionTracker::KeyUp(LayoutKey key) {
    LayoutKey result = {};
    unsigned long now = millis();  // Get the current time in milliseconds

    // check if we're tracking this key
    if (_keyData.count(key) == 0) {
        // we didn't know about this key, clear it out
        _keyData[key] = {now, 0, false};
    } else {
        auto& [startTime, pressCount, currentlyDown] = _keyData[key];

        currentlyDown = false;
    }
}

bool KeyActionTracker::GetKeyStatus(LayoutKey key, int count) {
     if (_keyData.count(key) == 0)
        return {};

    auto& [_, pressCount, currentlyDown] = _keyData[key];

    return currentlyDown && pressCount >= count;
}

void KeyActionTracker::Reset() {
    _keyData.clear();
}
