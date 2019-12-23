#include <Arduino.h>

#include "LatchButton.h"

LatchButton::LatchButton(int buttonPin)
    : _buttonPin(buttonPin) {
}

void LatchButton::initialize() {
    pinMode(_buttonPin, INPUT_PULLUP);
}

bool LatchButton::getAndClearState() {
    bool result = false;

    if (digitalRead(_buttonPin) == LOW) {
        if (!_buttonLatched) {
            _buttonLatched = true;
            result = true;
        }
    } else {
        _buttonLatched = false;
    }

    return result;
}
