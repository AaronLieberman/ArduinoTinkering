#pragma once

#include "Debouncer.h"

#include <string>
#include <vector>

class KeyScanner {
public:
    KeyScanner();

    void Init();

    bool Scan(std::vector<std::pair<int, int>> &outKeysDown, std::vector<std::pair<int, int>> &outKeysUp);

    void GetDebugKeys(std::vector<std::string> &outRows, std::vector<std::string> &outRowsSeen);
    const std::vector<uint32_t> &GetKeyPresses();

private:
    std::vector<std::vector<Debouncer>> _rows;
    uint32_t _lastHash = 0;
    std::vector<std::vector<uint8_t>> _rowsSeen;
};
