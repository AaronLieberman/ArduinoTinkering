#include "Encoder.h"

#include "SerialPrintf.h"

#include <Arduino.h>
#include <functional>

//#define TEST_SERIAL

static const int kDebounceMicros = 2000;

Encoder::Encoder(int encoderPinA, int encoderPinB, int encoderScale)
    : _encoderPinA(encoderPinA)
    , _encoderPinB(encoderPinB)
    , _encoderScale(encoderScale)
    , _valueA(kDebounceMicros, false)
    , _valueB(kDebounceMicros, false) {
}

void Encoder::initialize() {
    pinMode(_encoderPinA, INPUT_PULLUP);
    pinMode(_encoderPinB, INPUT_PULLUP);

    _valueA.setValue(digitalRead(_encoderPinA) == HIGH, true);
    _valueB.setValue(digitalRead(_encoderPinB) == HIGH, true);
}

bool Encoder::update() {
    bool changed = false;

    _valueA.setValue(digitalRead(_encoderPinA) == HIGH, true);
    _valueB.setValue(digitalRead(_encoderPinB) == HIGH, true);

    bool pinA = _valueA.getValue();
    bool pinB = _valueB.getValue();

    if (_lastValueA != pinA || _lastValueB != pinB) {
#ifdef TEST_SERIAL
        serialPrintfln("v: %s %s", pinA ? "A" : "_", pinB ? "B" : "_");
#endif
    }

    if (_lastValueA != pinA && _lastValueB == pinB) {
        if (pinA && !pinB) {
            _encoderPos--;
        } else if (!pinA && pinB) {
            _encoderPos--;
        } else if (pinA && pinB) {
            _encoderPos++;
        } else if (!pinA && !pinB) {
            _encoderPos++;
        }
    } else if (_lastValueB != pinB && _lastValueA == pinA) {
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

    _lastValueA = pinA;
    _lastValueB = pinB;

    if (_lastReportedPos != (_encoderPos / _encoderScale)) {
        changed = true;
        if (m_valueChanged != nullptr) {
            m_valueChanged((_encoderPos / _encoderScale) - _lastReportedPos);
        }

        _lastReportedPos = (_encoderPos / _encoderScale);
    }

    return changed;
}
