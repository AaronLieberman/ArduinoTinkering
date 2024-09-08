#include "KeyScanner.h"

#include <Arduino.h>

#include <Adafruit_MCP23X17.h>

const uint8_t kLeftCols = 7;
const uint8_t kLeftRowOffset = 8;
const uint8_t kRightCols = 9;
const uint8_t kRightRowOffset = 9;
const uint8_t kRows = 6;

bool _leftEnabled = false;
bool _rightEnabled = false;

Adafruit_MCP23X17 _ioPinsLeft;
Adafruit_MCP23X17 _ioPinsRight;

// KeyScanner

KeyScanner::KeyScanner() {
    _rows.resize(kRows);
    _rowsSeen.resize(kRows);
    for (auto &colVec : _rows) {
        colVec.resize(kLeftCols + kRightCols);
    }
    for (auto &colVec : _rowsSeen) {
        colVec.resize(kLeftCols + kRightCols, 255);
    }
}

void KeyScanner::Init() {
    _leftEnabled = _ioPinsLeft.begin_I2C(0x20);
    if (!_leftEnabled) {
        Serial.println("Failed to connect to left side I2C");
    }
    _rightEnabled = _ioPinsRight.begin_I2C(0x21);
    if (!_rightEnabled) {
        Serial.println("Failed to connect to right side I2C");
    }

    // initially clear all pins to output
    for (int i = 0; i < 16; i++) {
        if (_leftEnabled) {
            _ioPinsLeft.pinMode(i, OUTPUT);
        }

        if (_rightEnabled) {
            _ioPinsRight.pinMode(i, OUTPUT);
        }
    }

    if (_leftEnabled) {
        // left side uses 7 cols, 5 rows
        for (int i = 0; i < kLeftCols; i++) {
            _ioPinsLeft.pinMode(i, INPUT_PULLUP);
        }
        for (int i = kLeftRowOffset; i < kLeftRowOffset + kRows; i++) {
            _ioPinsLeft.pinMode(i, OUTPUT);
        }
    }

    if (_rightEnabled) {
        // right side uses 9 cols, 6 rows
        for (int i = 0; i < kRightCols; i++) {
            _ioPinsRight.pinMode(i, INPUT_PULLUP);
        }

        for (int i = kRightRowOffset; i < kRightRowOffset + kRows; i++) {
            _ioPinsRight.pinMode(i, OUTPUT);
        }
    }

    if (_leftEnabled) {
        _ioPinsLeft.writeGPIOA(255);
        _ioPinsLeft.writeGPIOB(255);
    }

    if (_rightEnabled) {
        _ioPinsRight.writeGPIOA(255);
        _ioPinsRight.writeGPIOB(255);
    }
}

bool KeyScanner::Scan(std::vector<std::pair<int, int>> &outKeysDown, std::vector<std::pair<int, int>> &outKeysUp) {
    outKeysDown.clear();
    outKeysUp.clear();
    uint32_t hash = 0;
    uint32_t seed = 131;

    for (int scanRowIndex = 0; scanRowIndex < kRows; scanRowIndex++) {
        if (_leftEnabled) {
            _ioPinsLeft.writeGPIOAB(~(1 << (kLeftRowOffset + scanRowIndex)));
        }
        if (_rightEnabled) {
            _ioPinsRight.writeGPIOAB(~(1 << (kRightRowOffset + scanRowIndex)));
        }

        uint16_t pinValuesLeft = _leftEnabled ? _ioPinsLeft.readGPIOAB() : 0xffff;
        uint16_t pinValuesRight = _rightEnabled ? _ioPinsRight.readGPIOAB() : 0xffff;

        int colIndex = 0;
        for (auto [cols, pinValues] :
            std::vector<std::pair<uint8_t, uint16_t>>{{kLeftCols, pinValuesLeft}, {kRightCols, pinValuesRight}}) {
            for (int i = 0; i < cols; i++) {
                bool pinValue = ((pinValues >> i) & 1) == 0;
                Debouncer &key = _rows[scanRowIndex][colIndex];
                bool orig = key.getValue();
                key.setValue(pinValue);
                bool cur = key.getValue();
                hash = (hash * seed) + (cur ? 1 : 0);

                bool changed = orig != cur;
                if (changed && cur) {
                    outKeysDown.push_back({scanRowIndex, colIndex});
                } else if (changed && !cur) {
                    outKeysUp.push_back({scanRowIndex, colIndex});
                }

                if (cur) {
                    _rowsSeen[scanRowIndex][colIndex] = (_rowsSeen[scanRowIndex][colIndex] + 1) % 10;
                }

                colIndex++;
            }
        }
    }

    bool changed = hash != _lastHash;
    _lastHash = hash;
    return changed;
}

void KeyScanner::GetDebugKeys(std::vector<std::string> &outRows, std::vector<std::string> &outRowsSeen) {
    outRows.clear();
    outRows.reserve(kRows);
    outRowsSeen.clear();
    outRowsSeen.reserve(kRows);

    auto accumulateRow = [](std::vector<Debouncer> &row, std::vector<uint8_t> &rowSeen, std::string &cols,
                             std::string &colsSeen, int colCount, int colOffset) {
        for (int colIndex = 0; colIndex < colCount; colIndex++) {
            cols += row[colIndex + colOffset].getValue() ? "x" : "-";
            colsSeen += rowSeen[colIndex + colOffset] == 255 ? '-' : ((rowSeen[colIndex + colOffset] % 10) + '0');
        }
    };

    for (int scanRowIndex = 0; scanRowIndex < kRows; scanRowIndex++) {
        std::string cols, colsSeen;
        cols.reserve(kLeftCols + kRightCols + 10);
        colsSeen.reserve(kLeftCols + kRightCols + 10);

        accumulateRow(_rows[scanRowIndex], _rowsSeen[scanRowIndex], cols, colsSeen, kLeftCols, 0);

        cols += " ";
        colsSeen += " ";

        accumulateRow(_rows[scanRowIndex], _rowsSeen[scanRowIndex], cols, colsSeen, kRightCols, kLeftCols);

        outRows.push_back(std::move(cols));
        outRowsSeen.push_back(std::move(colsSeen));
    }
}
