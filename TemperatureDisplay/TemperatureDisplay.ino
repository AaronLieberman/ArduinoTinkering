#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "AsyncTemperatureSensor.h"

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

// sensor pins
const int temperatureSensorPin = 8;

const int segTop = (1 << 7);
const int segDot = (1 << 1);
const int segMiddle = (1 << 5);
const int segBottomRight = (1 << 4);
const int segBottomLeft = (1 << 2);
const int segBottom = (1 << 3);
const int segTopLeft = (1 << 6);
const int segTopRight = (1 << 0);

const int decimals = 1;

byte segmentIndex = 0;
float valueToDisplay = 0;

AsyncTemperatureSensor sensor(temperatureSensorPin);

unsigned long lastUpdate = 0;

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

  pinMode(ledPin, OUTPUT);
  pinMode(hackPin, OUTPUT);
  pinMode(outputDisabledPin, OUTPUT);
  pinMode(registerClockPin, OUTPUT);  
  pinMode(whichSegmentData, OUTPUT);
  pinMode(segmentShiftRegisterClockPin, OUTPUT);  
  pinMode(whichDigitData, OUTPUT);
  pinMode(digitShiftRegisterClockPin, OUTPUT);
  pinMode(temperatureSensorPin, INPUT);

  digitalWrite(outputDisabledPin, LOW);
  digitalWrite(registerClockPin, LOW);
  digitalWrite(whichSegmentData, LOW);
  digitalWrite(segmentShiftRegisterClockPin, LOW);
  digitalWrite(whichDigitData, LOW);
  digitalWrite(digitShiftRegisterClockPin, LOW);

  sensor.start();
  
  valueToDisplay = 0;
  writeValue(0, valueToDisplay, false);
}

void loop()
{
  float place = calculatePlace(3 - segmentIndex, decimals);
  int segmentValue = ((int)(valueToDisplay / place)) % 10;

  writeValue(segmentIndex, segmentValue, segmentIndex == decimals + 1);

  segmentIndex = (segmentIndex + 1) % 4;

  float temperatureInF = sensor.getTempF();
  valueToDisplay = temperatureInF;

  unsigned long now = millis();
  if (now > lastUpdate + 100)
  {
    sensor.update();
    lastUpdate = now;
  }

  digitalWrite(ledPin, (now % 200 == 0) ? HIGH : LOW);
}

float calculatePlace(int segmentIndex, int decimals)
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
    default: assert(false);
  }
}

void writeValue(byte digit, byte value, bool showDot)
{
  int digitData = 1 << digit;
  int segmentData = digitValues[value] | (showDot ? segDot : 0);
  
  shiftOut(whichSegmentData, segmentShiftRegisterClockPin, MSBFIRST, segmentData);
  shiftOut(whichDigitData, digitShiftRegisterClockPin, MSBFIRST, digitData);

  digitalWrite(registerClockPin, HIGH);
  digitalWrite(registerClockPin, LOW);
}

