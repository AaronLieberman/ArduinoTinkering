#include <Arduino.h>
#include "ShiftRegister.h"

ShiftRegister::ShiftRegister(byte clockPin, byte latchPin, byte dataPin)
{
  _clockPin = clockPin;
  _latchPin = latchPin;
  _dataPin = dataPin;
}

void ShiftRegister::Initialize()
{
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);  
  pinMode(_latchPin, OUTPUT);

  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_latchPin, LOW);
  
  Write(0, true);
}

void ShiftRegister::Write(byte desiredState, bool latch)
{
  shiftOut(_dataPin, _clockPin, MSBFIRST, desiredState);

  if (latch)
  {
    digitalWrite(_latchPin, HIGH);
    digitalWrite(_latchPin, LOW);
  }
}

