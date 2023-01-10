#pragma once

enum class InputPinMode {
    Input = INPUT,
    InputPullDown = INPUT_PULLDOWN,
    InputPullUp = INPUT_PULLUP,
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
