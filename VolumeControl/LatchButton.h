#pragma once

class LatchButton {
public:
    LatchButton(int buttonPin);

    void initialize();
    bool getAndClearState();

private:
    int _buttonPin;
    bool _buttonLatched = false;
};
