#include <Arduino.h>

#include "LatchButton.h"

// LatchButton

LatchButton::LatchButton(int buttonPin, InputPinMode pinMode)
    : _buttonPin(buttonPin)
    , _pinMode(pinMode) {
}

void LatchButton::initialize() {
    pinMode(_buttonPin, (uint32_t)_pinMode);
}

bool LatchButton::getAndClearState() {
    bool result = false;

    int target = _pinMode == InputPinMode::Input || _pinMode == InputPinMode::InputPullUp ? LOW : HIGH;

    if (digitalRead(_buttonPin) == target) {
        if (!_buttonLatched) {
            _buttonLatched = true;
            result = true;
        }
    } else {
        _buttonLatched = false;
    }

    return result;
}

// CustomLatchButton

CustomLatchButton::CustomLatchButton(std::function<bool()> testFunc)
    : _testFunc(testFunc) {
}

void CustomLatchButton::initialize() {
}

bool CustomLatchButton::getAndClearState() {
    bool result = false;

    if (_testFunc()) {
        if (!_buttonLatched) {
            _buttonLatched = true;
            result = true;
        }
    } else {
        _buttonLatched = false;
    }

    return result;
}
