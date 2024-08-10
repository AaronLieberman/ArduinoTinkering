#pragma once

#include <functional>

enum class InputPinMode {
    Input,
    InputPullDown,
    InputPullUp,
};

class LatchButton {
public:
    LatchButton(int buttonPin, InputPinMode pinMode = InputPinMode::InputPullUp);

    void initialize();
    bool getAndClearState();

private:
    int _buttonPin;
    InputPinMode _pinMode;
    bool _buttonLatched = false;
};

class CustomLatchButton {
public:
    CustomLatchButton(std::function<bool()> testFunc);

    void initialize();
    bool getAndClearState();

private:
    std::function<bool()> _testFunc;
    bool _buttonLatched = false;
};
