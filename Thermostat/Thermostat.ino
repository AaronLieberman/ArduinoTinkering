#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "AsyncTemperatureSensor.h"
#include "LedDisplay.h"

const byte deviceLedPin = 13;
const byte heatPin = 4;
const byte heatOnPin = 3;
const byte registerClockPin = 11;
const byte whichSegmentDataPin = 9; 
const byte segmentShiftRegisterClockPin = 10;
const byte whichDigitDataPin = 6;
const byte digitShiftRegisterClockPin = 5;
const byte temperatureSensorPin = 8;

// keep the temperature between these values
const int lowTemp = 138;
const int highTemp = 142;

LedDisplay ledDisplay(segmentShiftRegisterClockPin, registerClockPin, whichSegmentDataPin,digitShiftRegisterClockPin, registerClockPin, whichDigitDataPin);
AsyncTemperatureSensor sensor(temperatureSensorPin);

void setup()
{
  ledDisplay.SetSegmentMapping(7, 5, 4, 2, 3, 6, 0, 1);
  ledDisplay.SetDecimals(1);

  pinMode(heatPin, OUTPUT);
  pinMode(heatOnPin, OUTPUT);
  pinMode(deviceLedPin, OUTPUT);

  sensor.Start();
  ledDisplay.Start();
}

void loop()
{
  sensor.Update();

  float temperatureInF = sensor.GetTempF();
  ledDisplay.SetValue(temperatureInF);
  ledDisplay.Update();

  if (temperatureInF < lowTemp)
  {
    digitalWrite(heatPin, HIGH);
    digitalWrite(heatOnPin, HIGH);
  }
  else if (temperatureInF > highTemp)
  {
    digitalWrite(heatPin, LOW);
    digitalWrite(heatOnPin, LOW);
  }
  
  digitalWrite(deviceLedPin, (millis() % 200 == 0) ? HIGH : LOW);
}

