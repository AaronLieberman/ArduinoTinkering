#include "LedDisplay.h"

LedDisplay::LedDisplay(byte anodeShiftRegisterClockPin, byte anodeRegisterClockPin, byte anodeDataPin, byte cathodeShiftRegisterClockPin, byte cathodeRegisterClockPin, byte cathodeDataPin) :
  _anodeShiftRegister(anodeShiftRegisterClockPin, anodeRegisterClockPin, anodeDataPin),
  _cathodeShiftRegister(cathodeShiftRegisterClockPin, cathodeRegisterClockPin, cathodeDataPin)
{
  memset(_digitValues, 0, sizeof(_digitValues));
}

void LedDisplay::SetSegmentMapping(byte top, byte middle, byte bottomRight, byte bottomLeft, byte bottom, byte topLeft, byte topRight, byte dot)
{
  top = 1 << top;
  middle = 1 << middle;
  bottomRight = 1 << bottomRight;
  bottomLeft = 1 << bottomLeft;
  bottom = 1 << bottom;
  topLeft = 1 << topLeft;
  topRight = 1 << topRight;
  dot = 1 << dot;
  memset(_digitValues, 0, sizeof(_digitValues));
  _digitValues[0] = top | topLeft | topRight | bottomLeft | bottomRight | bottom;
  _digitValues[1] = topRight | bottomRight;
  _digitValues[2] = top | topRight | middle | bottomLeft | bottom;
  _digitValues[3] = top | topRight | middle | bottomRight | bottom;
  _digitValues[4] = topLeft | topRight | middle | bottomRight;
  _digitValues[5] = top | topLeft | middle | bottomRight | bottom;
  _digitValues[6] = top | topLeft | middle | bottomLeft | bottomRight | bottom;
  _digitValues[7] = top | topRight | bottomRight;
  //_digitValues[8] = top | topLeft | topRight | middle | bottomLeft | bottomRight | bottom;
  //_digitValues[9] = top | topLeft | topRight | middle | bottomRight | bottom;
  _segDot = dot;
}

void LedDisplay::SetDecimals(byte decimalCount)
{
  _decimalCount = decimalCount;
}

void LedDisplay::Start()
{
  _anodeShiftRegister.Initialize();
  _cathodeShiftRegister.Initialize();
}

void LedDisplay::Update()
{
  float place = CalculatePlace(3 - _segmentIndex, _decimalCount);
  int segmentValue = ((int)(_valueToDisplay / place)) % 10;

  WriteValue(_segmentIndex, segmentValue, _segmentIndex == _decimalCount + 1);

  _segmentIndex = (_segmentIndex + 1) % 4;
}

void LedDisplay::WriteValue(byte digit, byte value, bool showDot)
{
  int digitData = 1 << digit;
  int segmentData = _digitValues[value] | (showDot ? _segDot : 0);

  _anodeShiftRegister.Write(segmentData, false);
  _cathodeShiftRegister.Write(digitData, true);
}

void LedDisplay::SetValue(float value)
{
  _valueToDisplay = value;
}

float LedDisplay::CalculatePlace(int segmentIndex, int decimals)
{
  // Arduino pow function is approximate. Easier to just use a LUT
  switch (segmentIndex - decimals)
  {
    case -3: return 0.001f;
    case -2: return 0.01f;
    case -1: return 0.1f;
    case 0: return 1;
    case 1: return 10;
    case 2: return 100;
    case 3: return 1000;
    default: return 0;
  }
}

