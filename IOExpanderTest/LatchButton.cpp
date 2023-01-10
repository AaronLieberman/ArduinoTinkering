#include <Arduino.h>

#include "LatchButton.h"

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
