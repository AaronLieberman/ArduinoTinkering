#pragma once
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
  int _lastRequestMillis = 0;
  bool _failure = false;

public:

  AsyncTemperatureSensor(int temperatureSensorPin);
  
  void start();
  void update();
  
  float getTempF();
  
};

