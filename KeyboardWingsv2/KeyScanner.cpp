#include "KeyScanner.h"
#include "SerialPrintf.h"

#include <Arduino.h>

#include <Adafruit_MCP23X17.h>

struct KeyScannerImpl {
    bool enabled = false;
    Adafruit_MCP23X17 ioPins;
};

// KeyScanner

KeyScanner::KeyScanner(uint8_t i2cAddr, uint8_t colCount, uint8_t rowOffset, uint8_t rowCount, uint8_t firstCol)
    : _impl(new KeyScannerImpl())
    , _i2cAddr(i2cAddr)
    , _colCount(colCount)
    , _rowOffset(rowOffset)
    , _rowCount(rowCount)
    , _firstCol(firstCol) {
    _rows.resize(_rowCount);
    _rowsSeen.resize(_rowCount);
    for (auto &colVec : _rows) {
        colVec.resize(_colCount);
    }
    for (auto &colVec : _rowsSeen) {
        colVec.resize(_colCount, 255);
    }
}

KeyScanner::~KeyScanner() {
    delete _impl;
}

void KeyScanner::Init() {
    Adafruit_MCP23X17 &ioPins = _impl->ioPins;

    _impl->enabled = ioPins.begin_I2C(_i2cAddr);
    if (!_impl->enabled) {
        serialPrintfln("Failed to connect to I2C at addr %d", _i2cAddr);
        return;
    }

    // initially clear all pins to output
    for (int i = 0; i < 16; i++) {
        ioPins.pinMode(i, OUTPUT);
    }

    for (int i = 0; i < _colCount; i++) {
        ioPins.pinMode(i, INPUT_PULLUP);
    }
    for (int i = _rowOffset; i < _rowOffset + _rowCount; i++) {
        ioPins.pinMode(i, OUTPUT);
    }

    ioPins.writeGPIOAB(0xffff);
}

bool KeyScanner::Scan(std::vector<std::pair<int, int>> &outKeysDown, std::vector<std::pair<int, int>> &outKeysUp) {
    if (!_impl->enabled) {
        return false;
    }

    Adafruit_MCP23X17 &ioPins = _impl->ioPins;

    outKeysDown.clear();
    outKeysUp.clear();

    _lastWasFastscan = false;

    uint32_t hash = 0;
    uint32_t seed = 131;

    for (int scanRowIndex = 0; scanRowIndex < _rowCount; scanRowIndex++) {
        ioPins.writeGPIOAB(~(1 << (_rowOffset + scanRowIndex)));
        uint16_t pinValues = ioPins.readGPIOAB();

        for (int colIndex = 0; colIndex < _colCount; colIndex++) {
            bool cur = ((pinValues >> colIndex) & 1) == 0;

            bool orig = _rows[scanRowIndex][colIndex];
            _rows[scanRowIndex][colIndex] = cur;
            hash = (hash * seed) + (cur ? 1 : 0);

            bool changed = orig != cur;
            if (changed && cur) {
                outKeysDown.push_back({scanRowIndex, colIndex + _firstCol});
            } else if (changed && !cur) {
                outKeysUp.push_back({scanRowIndex, colIndex + _firstCol});
            }

            if (cur) {
                _rowsSeen[scanRowIndex][colIndex] = (_rowsSeen[scanRowIndex][colIndex] + 1) % 10;
            }
        }
    }

    bool changed = hash != _lastHash;
    _lastHash = hash;
    return changed;
}

bool KeyScanner::FastScan() {
    if (!_impl->enabled) {
        return false;
    }

    Adafruit_MCP23X17 &ioPins = _impl->ioPins;

    if (!_lastWasFastscan) {
        ioPins.writeGPIOAB(0);
        _lastWasFastscan = true;
    }

    uint16_t pinValues = ioPins.readGPIOAB();
    for (int i = 0; i < _colCount; i++) {
        if (((pinValues >> i) & 1) == 0) {
            return true;
        }
    }

    return false;
}

void KeyScanner::GetDebugKeys(std::vector<std::string> &outRows, std::vector<std::string> &outRowsSeen) {
    outRows.clear();
    outRows.reserve(_rowCount);
    outRowsSeen.clear();
    outRowsSeen.reserve(_rowCount);

    for (int scanRowIndex = 0; scanRowIndex < _rowCount; scanRowIndex++) {
        std::string cols, colsSeen;
        cols.reserve(_colCount + 10);
        colsSeen.reserve(_colCount + 10);

        for (int colIndex = 0; colIndex < _colCount; colIndex++) {
            cols += _rows[scanRowIndex][colIndex] ? "x" : "-";
            colsSeen +=
                _rowsSeen[scanRowIndex][colIndex] == 255 ? '-' : ((_rowsSeen[scanRowIndex][colIndex] % 10) + '0');
        }

        outRows.push_back(std::move(cols));
        outRowsSeen.push_back(std::move(colsSeen));
    }
}
