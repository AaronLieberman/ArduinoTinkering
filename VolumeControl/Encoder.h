#pragma once

#include "stlhelper.h"

#include <functional>

class Encoder {
public:
    Encoder(int encoderPinA, int encoderPinB);

    void initialize();
    bool update();

    // When using this class, you need to register your own interrupts from outside the class and call these function
    // when the interrupts trigger
    void interruptATriggered();
    void interruptBTriggered();

    // valueChanged is always raised from within the Encoder's update function
    void setValueChanged(std::function<void(int)> valueChanged) { m_valueChanged = std::move(valueChanged); }

    int getPos() const { return _encoderPos; }
    void reset() { _encoderPos = 0; }

private:
    void _doEncoderA();
    void _doEncoderB();
    void _interruptTriggeredInternal(int pin1, bool& pin1Set, bool pin2Set, int mod);

    const int _encoderPinA;
    const int _encoderPinB;
    std::function<void(int)> m_valueChanged;
    bool _pinASet = false;
    bool _pinBSet = false;
    bool _rotating = false;
    volatile int _encoderPos = 0;
    unsigned int _lastReportedPos = 1;
};
