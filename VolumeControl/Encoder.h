#pragma once

#include "stlhelper.h"

#include <functional>

// This works as a low-resolution encoder. Doesn't require interrupts but needs to be updated quickly in your loop
// function
class Encoder {
public:
    Encoder(int encoderPinA, int encoderPinB);

    void initialize();
    bool update();

    // valueChanged is always raised from within the Encoder's update function
    void setValueChanged(std::function<void(int)> valueChanged) { m_valueChanged = std::move(valueChanged); }

    int getPos() const { return _encoderPos; }
    void reset() { _encoderPos = 0; }

private:
    const int _encoderPinA;
    const int _encoderPinB;
    std::function<void(int)> m_valueChanged;
    bool _lastPinA = false;
    unsigned int _lastPinAChange = 0;
    bool _lastPinB = false;
    unsigned int _lastPinBChange = 0;
    int _lastState = 0;
    int _encoderPos = 0;
    unsigned int _lastReportedPos = 0;
};
