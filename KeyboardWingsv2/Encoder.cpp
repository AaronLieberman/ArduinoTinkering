#include "Encoder.h"

#include "SerialPrintf.h"

#include <Arduino.h>
#include <algorithm>
#include <functional>

static const int kDebounceMicros = 2000;

std::vector<Encoder*> Encoder::_encoders;

// Index: [oldState(2 bits)][newState(2 bits)]
// Values: +1 => move CW, -1 => move CCW, 0 => no move
const int8_t _encoderLookup[16] = {
    // old=00 -> new=00,01,10,11
    0, 1, -1, 0,
    // old=01 -> new=00,01,10,11
    -1, 0, 0, 1,
    // old=10 -> new=00,01,10,11
    1, 0, 0, -1,
    // old=11 -> new=00,01,10,11
    0, -1, 1, 0};

Encoder::Encoder(int encoderPinA, int encoderPinB)
    : _encoderPinA(encoderPinA)
    , _encoderPinB(encoderPinB)
    , _valueA(kDebounceMicros, false)
    , _valueB(kDebounceMicros, false) {
}

Encoder::~Encoder() {
    const auto it = std::find(_encoders.begin(), _encoders.end(), this);
    if (it != _encoders.end()) {
        _encoders.erase(it);
    }
}

void Encoder::initialize() {
    pinMode(_encoderPinA, INPUT_PULLUP);
    pinMode(_encoderPinB, INPUT_PULLUP);

    _valueA.setValue(digitalRead(_encoderPinA) == HIGH, true);
    _valueB.setValue(digitalRead(_encoderPinB) == HIGH, true);

    uint8_t valueA = digitalRead(_encoderPinA);
    uint8_t valueB = digitalRead(_encoderPinB);
    _lastState = (valueA << 1) | valueB;

    attachInterrupt(digitalPinToInterrupt(_encoderPinA), &Encoder::handleInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_encoderPinB), &Encoder::handleInterrupt, CHANGE);

    _encoders.push_back(this);
}

void Encoder::update() {
    int scaledPos = (_encoderPos + 2) / 4;
    if (abs(_lastReportedPos - scaledPos) > 0) {
        if (m_valueChanged != nullptr) {
            m_valueChanged(scaledPos - _lastReportedPos);
        }

        _lastReportedPos = scaledPos;
    }
}

void Encoder::handleInterrupt() {
    for (auto it : _encoders) {
        it->handleInterruptInternal();
    }
}

void Encoder::handleInterruptInternal() {
    _valueA.setValue(digitalRead(_encoderPinA) == HIGH, true);
    _valueB.setValue(digitalRead(_encoderPinB) == HIGH, true);

    bool valueA = _valueA.getValue();
    bool valueB = _valueB.getValue();

    uint8_t newState = (valueA << 1) | valueB;
    uint8_t index = (_lastState << 2) | newState;
    _lastState = newState;

    int8_t direction = _encoderLookup[index];
    _encoderPos += direction;
}
