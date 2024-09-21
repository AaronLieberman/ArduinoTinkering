#pragma once

#include "KeyboardLayout.h"

#include <unordered_map>

class KeyActionTracker {
public:
    LayoutKey KeyDown(LayoutKey ke, int count);

private:
    const unsigned long kSuccessionDuration = 500;

    struct KeyInfo {
        unsigned long start_time;
        int press_count;
    };

    std::unordered_map<LayoutKey, KeyInfo> _keyData;
};
