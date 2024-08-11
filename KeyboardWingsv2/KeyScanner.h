#pragma once

#include <string>
#include <vector>

class KeyScanner {
public:
    KeyScanner();

    void Init();

    void Scan();

    std::vector<std::string> GetDebugKeys();
    const std::vector<uint32_t> &GetKeyPresses();

private:
    std::vector<std::vector<bool>> _rows;
};
