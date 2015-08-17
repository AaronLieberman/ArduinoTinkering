#include <Arduino.h>
#include "AsyncTemperatureSensor.h"

AsyncTemperatureSensor::AsyncTemperatureSensor(int temperatureSensorPin) :
    _oneWire(temperatureSensorPin),
    _sensor(&_oneWire)
{
}

void AsyncTemperatureSensor::Start()
{
  _sensor.begin();
}

void AsyncTemperatureSensor::Update()
{
  if (!_sensorAcquired && _sensor.getAddress(_deviceAddress, 0))
  {
    Serial.println("Acquired sensor.");
    
    _sensor.setWaitForConversion(false);
    _sensor.setResolution(12);
    
    if (_sensor.requestTemperaturesByAddress(_deviceAddress))
    {
      _sensorAcquired = true;
      _lastRequestMillis = millis();
    }
  }
  
  if (_sensorAcquired && millis() - _lastRequestMillis < 750 && _sensor.isConversionAvailable(_deviceAddress))
  {
     _temperatureInF = _sensor.getTempF(_deviceAddress);

    if (_sensor.requestTemperaturesByAddress(_deviceAddress))
    {
      _lastRequestMillis = millis();
    }
    else
    {
      _sensorAcquired = false;
    }
  }
}

float AsyncTemperatureSensor::GetTempF()
{
  return _temperatureInF;
}

