#pragma once
#include <Arduino.h>
#include "ShiftRegister.h"

class LedDisplay
{
private:

  byte _segDot;
  byte _segmentIndex = 0;
  float _valueToDisplay = 0;
  byte _decimalCount = 0;
  ShiftRegister _anodeShiftRegister;
  ShiftRegister _cathodeShiftRegister;
  byte _digitValues[10];

  float CalculatePlace(int segmentIndex, int decimals);
  void WriteValue(byte digit, byte value, bool showDot);

public:

  LedDisplay(byte anodeShiftRegisterClockPin, byte anodeRegisterClockPin, byte anodeDataPin, byte cathodeShiftRegisterClockPin, byte cathodeRegisterClockPin, byte cathodeDataPin);
  
  void SetSegmentMapping(byte top, byte dot, byte middle, byte bottomRight, byte bottomLeft, byte bottom, byte topLeft, byte topRight);
  void SetDecimals(byte decimalCount);
  
  void Start();
  void Update();

  void SetValue(float value);

};

