#include <arduino.h>

// led on device
const int ledPin = 13;
const int hackPin = 8;

// shared pins
const int outputDisabledPin = 3;
const int registerClockPin = 11;

// segment pins
const int whichSegmentData = 9; 
const int segmentShiftRegisterClockPin = 10;

// digit pins
const int whichDigitData = 6;
const int digitShiftRegisterClockPin = 5;

const int segTop = (1 << 7);
const int segDot = (1 << 1);
const int segMiddle = (1 << 5);
const int segBottomRight = (1 << 4);
const int segBottomLeft = (1 << 2);
const int segBottom = (1 << 3);
const int segTopLeft = (1 << 6);
const int segTopRight = (1 << 0);

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
  //Serial.begin(9600);
  //Serial.println();

  pinMode(ledPin, OUTPUT);
  pinMode(hackPin, OUTPUT);
  pinMode(outputDisabledPin, OUTPUT);
  pinMode(registerClockPin, OUTPUT);  
  pinMode(whichSegmentData, OUTPUT);
  pinMode(segmentShiftRegisterClockPin, OUTPUT);  
  pinMode(whichDigitData, OUTPUT);
  pinMode(digitShiftRegisterClockPin, OUTPUT);

  digitalWrite(outputDisabledPin, LOW);
  digitalWrite(registerClockPin, LOW);
  digitalWrite(whichSegmentData, LOW);
  digitalWrite(segmentShiftRegisterClockPin, LOW);
  digitalWrite(whichDigitData, LOW);
  digitalWrite(digitShiftRegisterClockPin, LOW);
  
  //shiftOut(whichSegmentData, segmentShiftRegisterClockPin, MSBFIRST, 255);
  //shiftOut(whichDigitData, digitShiftRegisterClockPin, MSBFIRST, 0);
  //digitalWrite(registerClockPin, HIGH);
  //digitalWrite(registerClockPin, LOW);

  valueToDisplay = 1234;
  writeValue(0, valueToDisplay);
}

bool toggle = false;
unsigned long lastToggle = 0;

void loop2()
{
  unsigned long now = millis();

  if (now - lastToggle > 3000)
  {
    toggle = !toggle;
    lastToggle = now;
  }

  unsigned long pinValue = toggle ? HIGH : LOW;
  digitalWrite(ledPin, pinValue);
  digitalWrite(hackPin, pinValue);
  //digitalWrite(registerClockPin, pinValue);
  //digitalWrite(whichSegmentData, pinValue);
  //digitalWrite(segmentShiftRegisterClockPin, pinValue);
  //digitalWrite(whichDigitData, pinValue);
  //digitalWrite(digitShiftRegisterClockPin, pinValue);
}

void loop()
{
  // Arduino pow function is approximate and sometimes will end up rounding down when casting to int
  int place = pow(10, 3 - segmentIndex) + 0.5f;
  int segmentValue = (valueToDisplay / place) % 10;

  writeValue(segmentIndex, segmentValue);

  segmentIndex = (segmentIndex + 1) % 4;

  unsigned long now = millis();
  if (now > lastIncrement + 20)
  {
    valueToDisplay = (valueToDisplay + 1) % 10000;
    lastIncrement = now;
  }

  digitalWrite(ledPin, (now % 200 == 0) ? HIGH : LOW);
}

void writeValue(byte digit, byte value)
{
  int digitData = 1 << digit;
  int segmentData = digitValues[value];
  
  shiftOut(whichSegmentData, segmentShiftRegisterClockPin, MSBFIRST, segmentData);
  shiftOut(whichDigitData, digitShiftRegisterClockPin, MSBFIRST, digitData);

  digitalWrite(registerClockPin, HIGH);
  digitalWrite(registerClockPin, LOW);
}

