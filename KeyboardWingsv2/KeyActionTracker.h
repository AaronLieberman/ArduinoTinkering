#pragma once

#include "KeyboardLayout.h"

#include <unordered_map>

class KeyActionTracker {
public:
    void KeyDown(LayoutKey key);
    void KeyUp(LayoutKey key);
    bool GetKeyStatus(LayoutKey key, int count);
    void Reset();

private:
    const unsigned long kRepeatWindowMs = 500;

    struct KeyInfo {
        unsigned long startTime;
        int pressCount;
        bool currentlyDown;
    };

    std::unordered_map<LayoutKey, KeyInfo> _keyData;
};
