#pragma once
#include <Arduino.h>

class ShiftRegister
{
private:

  byte _dataPin;
  byte _clockPin;
  byte _latchPin;
  byte shiftRegisterData;

public:

  ShiftRegister(byte clockPin, byte latchPin, byte dataPin);

  void Initialize();
  void Write(byte desiredState, bool latch);

};

