#include <arduino.h>

// shared pins
int outputDisabledPin = 9;
int registerClockPin = 10;

// segment pins
int whichSegmentData = 2; 
int segmentShiftRegisterClockPin = 4;

// digit pins
int whichDigitData = 11;
int shiftRegisterClockPin = 12;

const int segTop = (1 << 0);
const int segDot = (1 << 1);
const int segMiddle = (1 << 2);
const int segBottomRight = (1 << 3);
const int segBottomLeft = (1 << 4);
const int segBottom = (1 << 5);
const int segTopLeft = (1 << 6);
const int segTopRight = (1 << 7);

byte segmentIndex = 0;
int valueToDisplay = 0;

unsigned long lastIncrement = 0;

int digitValues[] = 
{
  segTop | segTopLeft | segTopRight | segBottomLeft | segBottomRight | segBottom,
  segTopRight | segBottomRight,
  segTop | segTopRight | segMiddle | segBottomLeft | segBottom,
  segTop | segTopRight | segMiddle | segBottomRight | segBottom,
  segTopLeft | segTopRight | segMiddle | segBottomRight,
  segTop | segTopLeft | segMiddle | segBottomRight | segBottom,
  segTop | segTopLeft | segMiddle | segBottomLeft | segBottomRight | segBottom,
  segTop | segTopRight | segBottomRight,
  segTop | segTopLeft | segTopRight | segMiddle | segBottomLeft | segBottomRight | segBottom,
  segTop | segTopLeft | segTopRight | segMiddle | segBottomRight | segBottom,
};

void setup()
{
  Serial.begin(9600);
  Serial.println();
  
  pinMode(outputDisabledPin, OUTPUT);
  pinMode(registerClockPin, OUTPUT);  
  pinMode(whichSegmentData, OUTPUT);
  pinMode(segmentShiftRegisterClockPin, OUTPUT);  
  pinMode(whichDigitData, OUTPUT);
  pinMode(shiftRegisterClockPin, OUTPUT);

  digitalWrite(outputDisabledPin, LOW);

  writeValue(0, 0);
  valueToDisplay = 0;
}

void loop()
{
  // Arduino pow function is approximage and sometimes will end up rounding down when casting to int
  int place = pow(10, 3 - segmentIndex) + 0.5f;
  int segmentValue = (valueToDisplay / place) % 10;

  writeValue(segmentIndex, segmentValue);

  segmentIndex = (segmentIndex + 1) % 4;

  unsigned long now = millis();
  if (now > lastIncrement + 20)
  {
    valueToDisplay++;
    lastIncrement = now;
  }
}

void writeValue(byte digit, byte value)
{
  int digitData = 1 << digit;
  int segmentData = digitValues[value];
  
  shiftOut(whichSegmentData, segmentShiftRegisterClockPin, MSBFIRST, segmentData);
  shiftOut(whichDigitData, shiftRegisterClockPin, MSBFIRST, digitData);

  digitalWrite(registerClockPin, HIGH);
  digitalWrite(registerClockPin, LOW);
}

