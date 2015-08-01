#pragma once

class ShiftRegister
{
private:

  int _dataPin;
  int _clockPin;
  int _latchPin;
  int shiftRegisterData;

public:

  ShiftRegister(int dataPin, int clockPin, int latchPin);

  void initialize();
  void write(int desiredPin, boolean desiredState);

};

