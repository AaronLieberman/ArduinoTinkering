#pragma once

#include "stlhelper.h"
#include "Debouncer.h"

#include <functional>

// This works as a low-resolution encoder. Doesn't require interrupts but needs to be updated quickly in your loop
// function
class Encoder {
public:
    Encoder(int encoderPinA, int encoderPinB, int encoderScale);

    void initialize();
    bool update();

    // valueChanged is always raised from within the Encoder's update function
    void setValueChanged(std::function<void(int)> valueChanged) { m_valueChanged = std::move(valueChanged); }

    int getPos() const { return _encoderPos; }
    void reset() { _encoderPos = 0; }

private:
    const int _encoderPinA;
    const int _encoderPinB;
    const int _encoderScale;
    std::function<void(int)> m_valueChanged;
    Debouncer<bool> _valueA;
    Debouncer<bool> _valueB;
    bool _lastValueA = false;
    bool _lastValueB = false;
    int _lastState = 0;
    int _encoderPos = 0;
    unsigned int _lastReportedPos = 0;
};
