#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "ShiftRegister.h"
#include "AsyncTemperatureSensor.h"
#include "LatchButton.h"

const int dataPin = 2; 
const int clockPin = 3;
const int latchPin = 4;
const int warningPin = 5;
const int potentiometerPin = 0;
const int temperatureSensorPin = 8;
const int buttonPin = 9;
const int heatPin = 10;

// keep the temperature between these values
const int lowTemp = 138;
const int highTemp = 142;

AsyncTemperatureSensor sensor(temperatureSensorPin);
LatchButton button(buttonPin);
ShiftRegister shiftRegister(dataPin, clockPin, latchPin);

bool usePotentiometer = false;

int lastPrintMilli = 0;
bool printThisFrame;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Hi");
  printMode();

  shiftRegister.initialize();
  button.initialize();
  pinMode(warningPin, OUTPUT);
  pinMode(heatPin, OUTPUT);

  sensor.start();
}

void loop()
{
  printThisFrame = false;
  int now = millis();
  if (now - lastPrintMilli > 1000)
  {
    printThisFrame = true;
    lastPrintMilli = now;
  }
  
  float temperatureInF = 0;

  if (usePotentiometer)
  {
    int sensorValue = analogRead(potentiometerPin); 
    // sensorValue goes from 0 to 1023
    // let's reverse it and map it from 0 to 150
    temperatureInF = ((float)(1023 - sensorValue) * 150) / 1024;
    printlnPeriodic(String(sensorValue) + ", " + String(temperatureInF));
  }
  else
  {
    int temperatureInF = sensor.getTempF();
    printlnPeriodic("Temp: " + String(temperatureInF));
  }

  if (button.getAndClearState())
  {
      usePotentiometer = !usePotentiometer;
      printMode();
  }
  
  showTemperature((int)temperatureInF);

  if (temperatureInF < lowTemp)
  {
    digitalWrite(heatPin, HIGH);
  }
  else if (temperatureInF > highTemp)
  {
    digitalWrite(heatPin, LOW);
  }
}

void printMode()
{
  if (usePotentiometer) Serial.println("Now using potentiometer.");
  if (!usePotentiometer) Serial.println("Now using temperature sensor.");
}

void showTemperature(int value)
{
  int ledCount = (value - 100) / 5;
  ledCount = max(ledCount, 0);
  ledCount = min(ledCount, 8);

  for (int i = 0; i < 8; i++)
  {
    shiftRegister.write(i, i < ledCount ? HIGH : LOW);
  }

  if (value >= 145 && (millis() % 150) < 100)
  {
    digitalWrite(warningPin, HIGH);
  }
  else
  {
    digitalWrite(warningPin, LOW);
  }
}

void printlnPeriodic(String s)
{
  if (!printThisFrame) return;
  
  Serial.println(s);
}


