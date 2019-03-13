#pragma once

enum InputPinMode
{
	Normal = INPUT,
	PullDown = INPUT_PULLDOWN,
};

class LatchButton
{
public:

  LatchButton(int buttonPin, InputPinMode pinMode, int latchInterval = 0);

  void initialize();
  bool getAndClearState();

private:

  int _buttonPin;
  InputPinMode _pinMode;
  int _latchInterval;
  bool _buttonLatched = false;
  int _lastLatched = 0;
  
};
