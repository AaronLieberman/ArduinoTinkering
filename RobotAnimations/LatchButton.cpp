#include <Arduino.h>

#include "LatchButton.h"

LatchButton::LatchButton(int buttonPin, InputPinMode pinMode, int latchInterval)
	: _buttonPin(buttonPin)
	, _pinMode(pinMode)
	, _latchInterval(latchInterval)
{
}

void LatchButton::initialize()
{
	pinMode(_buttonPin, _pinMode);
}

bool LatchButton::getAndClearState()
{
	bool result = false;

	if (digitalRead(_buttonPin) == HIGH)
	{
		if (!_buttonLatched && (_latchInterval == 0 || ((millis() - _lastLatched) >= _latchInterval)))
		{
			_buttonLatched = true;
			result = true;
			_lastLatched = millis();
		}
	}
	else
	{
		_buttonLatched = false;
	}

	return result;
}
