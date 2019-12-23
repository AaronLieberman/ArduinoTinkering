#include "Encoder.h"

#include <Arduino.h>
#include <functional>

static const int kDebounceMicros = 2000;

Encoder::Encoder(int encoderPinA, int encoderPinB)
    : _encoderPinA(encoderPinA)
    , _encoderPinB(encoderPinB) {
}

void Encoder::initialize() {
    pinMode(_encoderPinA, INPUT_PULLUP);
    pinMode(_encoderPinB, INPUT_PULLUP);

    _lastPinA = digitalRead(_encoderPinA);
    _lastPinB = digitalRead(_encoderPinB);
}

bool Encoder::update() {
    bool changed = false;

    uint now = micros();

    bool pinA = _lastPinA;
    if (now >= _lastPinAChange + kDebounceMicros) {
        _lastPinAChange = now;
        pinA = digitalRead(_encoderPinA);
    }

    bool pinB = _lastPinB;
    if (now >= _lastPinBChange + kDebounceMicros) {
        _lastPinBChange = now;
        pinB = digitalRead(_encoderPinB);
    }

    if (_lastPinA != pinA && _lastPinB == pinB) {
        if (pinA && !pinB) {
            _encoderPos--;
        } else if (!pinA && pinB) {
            _encoderPos--;
        } else if (pinA && pinB) {
            _encoderPos++;
        } else if (!pinA && !pinB) {
            _encoderPos++;
        }
    } else if (_lastPinB != pinB && _lastPinA == pinA) {
        if (pinA && pinB) {
            _encoderPos--;
        } else if (!pinA && !pinB) {
            _encoderPos--;
        } else if (pinA && !pinB) {
            _encoderPos++;
        } else if (!pinA && pinB) {
            _encoderPos++;
        }
    }

    _lastPinA = pinA;
    _lastPinB = pinB;

    if (_lastReportedPos != _encoderPos) {
        changed = true;
        if (m_valueChanged != nullptr) {
            m_valueChanged(_encoderPos - _lastReportedPos);
        }

        _lastReportedPos = _encoderPos;
    }

    return changed;
}
