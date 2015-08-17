#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class AsyncTemperatureSensor
{
private:

  OneWire _oneWire;
  DallasTemperature _sensor;
  
  DeviceAddress _deviceAddress;
  bool _sensorAcquired = false;
  float _temperatureInF = 0;
  unsigned long _lastRequestMillis = 0;
  bool _failure = false;

public:

  AsyncTemperatureSensor(byte temperatureSensorPin);
  
  void Start();
  void Update();
  
  float GetTempF();
  
};

