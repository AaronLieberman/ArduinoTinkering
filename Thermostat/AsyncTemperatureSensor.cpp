#include <Arduino.h>
#include "AsyncTemperatureSensor.h"

AsyncTemperatureSensor::AsyncTemperatureSensor(byte temperatureSensorPin) :
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
  // aquire sensor
  if (!_sensorAcquired && _sensor.getAddress(_deviceAddress, 0))
  {
    _sensor.setWaitForConversion(false);
    _sensor.setResolution(12);
    
    if (_sensor.requestTemperaturesByAddress(_deviceAddress))
    {
      _sensorAcquired = true;
      _lastRequestMillis = millis();
    }
  }

  // get temperature
  // 750 millis is how long it takes to retrieve the temperature from the sensor. Let's give it room and call it 1000ms
  if (_sensorAcquired && (millis() - _lastRequestMillis > 1000))
  {
    _temperatureInF = _sensor.getTempF(_deviceAddress);
    _lastRequestMillis = millis();

    if (!_sensor.requestTemperaturesByAddress(_deviceAddress))
    {
      _sensorAcquired = false;
    }
  }
}

float AsyncTemperatureSensor::GetTempF()
{
  return _temperatureInF;
}

