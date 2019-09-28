#include <Arduino.h>

#include "PeriodicDebug.h"

PeriodicDebug::PeriodicDebug(int blinkInterval, int logInterval)
	: _blinkInterval(blinkInterval)
	, _logInterval(logInterval)
{
}

void PeriodicDebug::initialize()
{
	if (_blinkInterval > 0)
	{
		pinMode(LED_BUILTIN, OUTPUT);
	}
}

void PeriodicDebug::update()
{
	if (_logInterval > 0 && millis() >= _lastLogMillis + _logInterval)
	{
		Serial.print(".");
		_lastLogMillis = millis();
	}

	if (_blinkInterval > 0 && millis() >= _lastBlinkMillis + _blinkInterval)
	{
		_lastBlinkState = !_lastBlinkState;
		digitalWrite(LED_BUILTIN, _lastBlinkState ? HIGH : LOW);
		_lastBlinkMillis = millis();
	}
}
