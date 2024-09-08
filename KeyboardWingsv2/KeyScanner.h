#pragma once

#include "Debouncer.h"

#include <string>
#include <vector>

enum class KeyboardSide { None = 0, Left = 1, Right = 2, Both = Left | Right };

class KeyScanner {
public:
    KeyScanner();

    void Init();

    KeyboardSide FastScan();
    bool Scan(KeyboardSide sidesToScan, std::vector<std::pair<int, int>> &outKeysDown,
        std::vector<std::pair<int, int>> &outKeysUp);

    void GetDebugKeys(std::vector<std::string> &outRows, std::vector<std::string> &outRowsSeen);

private:
    std::vector<std::vector<Debouncer>> _rows;
    uint32_t _lastHash = 0;
    bool _lastWasFastscan = false;
    std::vector<std::vector<uint8_t>> _rowsSeen;
};
