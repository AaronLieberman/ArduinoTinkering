#pragma once

#include "Debouncer.h"
#include "stlhelper.h"

#include <functional>

// This works as a low-resolution encoder. Doesn't require interrupts but needs to be updated quickly in your loop
// function
class Encoder {
public:
    Encoder(int encoderPinA, int encoderPinB);
    ~Encoder();

    void initialize();

    // fires value changed callbacks
    void update();

    // valueChanged is always raised from within the Encoder's update function
    void setValueChanged(std::function<void(int)> valueChanged) { m_valueChanged = std::move(valueChanged); }

    int getPos() const { return _encoderPos; }
    void reset() { _encoderPos = 0; }

private:
    // called when any interrupt from any encoder occurs
    static void handleInterrupt();

    // encoders that need to be notified of interrupts
    static std::vector<Encoder*> _encoders;

    // called when an interrupt occurs
    void handleInterruptInternal();

    const int _encoderPinA;
    const int _encoderPinB;
    std::function<void(int)> m_valueChanged;
    Debouncer<bool> _valueA;
    Debouncer<bool> _valueB;
    uint8_t _lastState = 0;
    int _encoderPos = 0;
    unsigned int _lastReportedPos = 0;
};
