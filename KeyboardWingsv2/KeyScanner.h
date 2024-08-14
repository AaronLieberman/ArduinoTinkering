#pragma once

#include "Debouncer.h"

#include <string>
#include <vector>

class KeyScanner {
public:
    KeyScanner();

    void Init();

    bool Scan();

    std::vector<std::string> GetDebugKeys();
    const std::vector<uint32_t> &GetKeyPresses();

private:
    std::vector<std::vector<Debouncer>> _rows;
    uint32_t _lastHash = 0;
};
