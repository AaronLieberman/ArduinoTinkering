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
    for (auto &colVec : _rows) {
        colVec.resize(kLeftCols + kRightCols);
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
        _ioPinsLeft.pinMode(i, OUTPUT);
        _ioPinsRight.pinMode(i, OUTPUT);
    }

    // left side uses 7 cols, 5 rows
    for (int i = 0; i < kLeftCols; i++) {
        _ioPinsLeft.pinMode(i, INPUT_PULLUP);
    }
    for (int i = kLeftRowOffset; i < kLeftRowOffset + kRows; i++) {
        _ioPinsLeft.pinMode(i, OUTPUT);
    }

    // right side uses 9 cols, 6 rows
    for (int i = 0; i < kRightCols; i++) {
        _ioPinsRight.pinMode(i, INPUT_PULLUP);
    }

    for (int i = kRightRowOffset; i < kRightRowOffset + kRows; i++) {
        _ioPinsRight.pinMode(i, OUTPUT);
    }

    _ioPinsLeft.writeGPIOA(255);
    _ioPinsLeft.writeGPIOB(255);
    _ioPinsRight.writeGPIOA(255);
    _ioPinsRight.writeGPIOB(255);
}

void KeyScanner::Scan() {
    for (int scanRowIndex = 0; scanRowIndex < kRows; scanRowIndex++) {
        for (int rowBit = 0; rowBit < kRows; rowBit++) {
            _ioPinsLeft.digitalWrite(kLeftRowOffset + rowBit, scanRowIndex != rowBit);
            _ioPinsRight.digitalWrite(kRightRowOffset + rowBit, scanRowIndex != rowBit);
        }

        uint16_t pinValuesLeft = (_ioPinsLeft.readGPIOB() << 8) | _ioPinsLeft.readGPIOA();
        uint16_t pinValuesRight = (_ioPinsRight.readGPIOB() << 8) | _ioPinsRight.readGPIOA();

        int colIndex = 0;
        for (int i = 0; i < kLeftCols; i++) {
            bool pinValue = ((pinValuesLeft >> i) & 1) > 0;
            _rows[scanRowIndex][colIndex++] = pinValue;
        }

        for (int i = 0; i < kRightCols; i++) {
            bool pinValue = ((pinValuesRight >> i) & 1) > 0;
            _rows[scanRowIndex][colIndex++] = pinValue;
        }
    }
}

std::vector<std::string> KeyScanner::GetDebugKeys() {
    std::vector<std::string> rows;
    
    for (int scanRowIndex = 0; scanRowIndex < kRows; scanRowIndex++) {
        std::string cols;
        cols.reserve(kLeftCols + kRightCols + 10);

        for (int colIndex = 0; colIndex < kLeftCols; colIndex++) {
            cols += _rows[scanRowIndex][colIndex] ? "-" : "x";
        }

        cols += " ";

        for (int colIndex = 0; colIndex < kRightCols; colIndex++) {
            cols += _rows[scanRowIndex][kLeftCols + colIndex] ? "-" : "x";
        }

        rows.push_back(std::move(cols));
    }

    return rows;
}

const std::vector<uint32_t> &KeyScanner::GetKeyPresses() {
    
    return std::vector<uint32_t>();
}
