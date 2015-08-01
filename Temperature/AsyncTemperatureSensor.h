#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>

class AsyncTemperatureSensor
{
private:

  OneWire _oneWire;
  DallasTemperature _sensors;
  
  DeviceAddress _deviceAddress;
  bool _sensorAcquired = false;
  int _temperatureInF = 0;

public:

  AsyncTemperatureSensor(int temperatureSensorPin);
  
  void start();

  int getTempF();
  
};

