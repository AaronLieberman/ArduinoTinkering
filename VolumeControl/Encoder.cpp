#include "Encoder.h"

#include <Arduino.h>
#include <functional>

Encoder::Encoder(int encoderPinA, int encoderPinB)
    : _encoderPinA(encoderPinA)
    , _encoderPinB(encoderPinB) {
}

void Encoder::initialize() {
    pinMode(_encoderPinA, INPUT_PULLUP);
    pinMode(_encoderPinB, INPUT_PULLUP);
}

bool Encoder::update() {
    bool changed = false;
    _rotating = true;  // reset the debouncer

    if (_lastReportedPos != _encoderPos) {
        changed = true;
        if (m_valueChanged != nullptr) {
            m_valueChanged(_encoderPos - _lastReportedPos);
        }

        _lastReportedPos = _encoderPos;
    }

    return changed;
}

void Encoder::_interruptTriggeredInternal(int pin1, bool& pin1Set, bool pin2Set, int mod) {
    // debounce
    if (_rotating) {
        delay(1);  // wait a little until the bouncing is done
    }

    // Test transition, did things really change?
    if (digitalRead(pin1) != pin1Set) {  // debounce once more
        pin1Set = !pin1Set;

        // adjust counter + if the first pin leads the second pin
        if (pin1Set && !pin2Set) {
            _encoderPos += mod;
        }

        _rotating = false;  // no more debouncing until loop() hits again
    }
}

void Encoder::interruptATriggered() {
    _interruptTriggeredInternal(_encoderPinA, _pinASet, _pinBSet, -1);
}

void Encoder::interruptBTriggered() {
    _interruptTriggeredInternal(_encoderPinB, _pinBSet, _pinASet, 1);
}
