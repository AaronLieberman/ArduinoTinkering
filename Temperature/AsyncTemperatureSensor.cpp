#include <Arduino.h>
#include "AsyncTemperatureSensor.h"

AsyncTemperatureSensor::AsyncTemperatureSensor(int temperatureSensorPin) :
    _oneWire(temperatureSensorPin),
    _sensors(&_oneWire)
{
}

void AsyncTemperatureSensor::start()
{
  _sensors.begin();
}

int AsyncTemperatureSensor::getTempF()
{
  if (!_sensorAcquired && _sensors.getAddress(_deviceAddress, 0))
  {
    Serial.println("Acquired sensor.");
    
    _sensors.setWaitForConversion(false);
    if (_sensors.requestTemperaturesByAddress(_deviceAddress))
    {
      _sensorAcquired = true;
    }
  }
  
  if (_sensorAcquired && _sensors.isConversionAvailable(_deviceAddress))
  {
     _temperatureInF = _sensors.getTempF(_deviceAddress);

    if (!_sensors.requestTemperaturesByAddress(_deviceAddress))
    {
      _sensorAcquired = false;
    }
  }

  return _temperatureInF;
}

