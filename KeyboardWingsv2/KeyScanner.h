#pragma once

#include <string>
#include <vector>

enum class KeyboardSide { None = 0, Left = 1, Right = 2, Both = Left | Right };

class KeyScanner {
public:
    KeyScanner(uint8_t i2cAddr, uint8_t colCount, uint8_t rowOffset, uint8_t rowCount, uint8_t firstCol);
    ~KeyScanner();

    KeyScanner(const KeyScanner &) = delete;
    KeyScanner &operator=(const KeyScanner &) = delete;

    void Init();

    bool FastScan();
    bool Scan(bool fastScanResult, std::vector<std::pair<int, int>> &outKeysDown,
        std::vector<std::pair<int, int>> &outKeysUp);
    void Clear();

    void GetDebugKeys(std::vector<std::string> &outRows, std::vector<std::string> &outRowsSeen);

private:
    class KeyScannerImpl *_impl;

    uint8_t _i2cAddr = 0x20;
    uint8_t _colCount = 0;
    uint8_t _rowOffset = 0;
    uint8_t _rowCount = 0;
    uint8_t _firstCol = 0;

    std::vector<std::vector<bool>> _rows;
    uint32_t _lastHash = 0;
    bool _lastWasFastscan = false;
    std::vector<std::vector<uint8_t>> _rowsSeen;
};
