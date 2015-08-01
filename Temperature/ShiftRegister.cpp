#include <Arduino.h>
#include "ShiftRegister.h"

ShiftRegister::ShiftRegister(int dataPin, int clockPin, int latchPin) :
  _dataPin(dataPin),
  _clockPin(clockPin),
  _latchPin(latchPin)
{
}

void ShiftRegister::initialize()
{
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);  
  pinMode(_latchPin, OUTPUT);
}

void ShiftRegister::write(int desiredPin, boolean desiredState)
{
  bitWrite(shiftRegisterData, desiredPin, desiredState);
  
  shiftOut(_dataPin, _clockPin, MSBFIRST, shiftRegisterData);

  digitalWrite(_latchPin, HIGH);
  digitalWrite(_latchPin, LOW);
}

