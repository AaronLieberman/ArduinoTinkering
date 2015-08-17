#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "AsyncTemperatureSensor.h"
#include "LedDisplay.h"

const int deviceLedPin = 13;
const int heatPin = 4;
const int heatOnPin = 3;
const int registerClockPin = 11;
const int whichSegmentDataPin = 9; 
const int segmentShiftRegisterClockPin = 10;
const int whichDigitDataPin = 6;
const int digitShiftRegisterClockPin = 5;
const int temperatureSensorPin = 8;

LedDisplay ledDisplay(segmentShiftRegisterClockPin, registerClockPin, whichSegmentDataPin,digitShiftRegisterClockPin, registerClockPin, whichDigitDataPin);
AsyncTemperatureSensor sensor(temperatureSensorPin);

unsigned long lastUpdate = 0;

void setup()
{
  ledDisplay.SetSegmentMapping(7, 5, 4, 2, 3, 6, 0, 1);
  ledDisplay.SetDecimals(1);

  pinMode(deviceLedPin, OUTPUT);
  digitalWrite(deviceLedPin, LOW);

  sensor.Start();
  ledDisplay.Start();
}

void loop()
{
  float temperatureInF = sensor.GetTempF();
  ledDisplay.SetValue(temperatureInF);
  ledDisplay.Update();

  unsigned long now = millis();
  if (now > lastUpdate + 100)
  {
    sensor.Update();
    lastUpdate = now;
  }

  digitalWrite(deviceLedPin, (now % 200 == 0) ? HIGH : LOW);
}

