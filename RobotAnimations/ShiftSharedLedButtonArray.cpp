#include <Arduino.h>

#include "ShiftSharedLedButtonArray.h"

constexpr int kTriggerInterval = 300;

ShiftSharedLedButtonArray::ShiftSharedLedButtonArray(int shiftLatchPin, int shiftClockPin, int shiftDataPin,
	int readPin, const std::vector<ShiftSharedLedButtonConfig>& config)
	: _shiftLatchPin(shiftLatchPin)
	, _shiftClockPin(shiftClockPin)
	, _shiftDataPin(shiftDataPin)
	, _readPin(readPin)
	, _config(config)
{
}

void ShiftSharedLedButtonArray::initialize()
{
	pinMode(_shiftLatchPin, OUTPUT);
	pinMode(_shiftClockPin, OUTPUT);
	pinMode(_shiftDataPin, OUTPUT);
	pinMode(_readPin, INPUT);
	applyLedState();
}

void ShiftSharedLedButtonArray::clearLedState()
{
	_ledState = 0;
}

void ShiftSharedLedButtonArray::setLedState(int ledIndex, bool state)
{
	int ledMask = 1 << _config[ledIndex].ledBit;
	_ledState = (_ledState & ~ledMask) | (state ? ledMask : 0);
}

void ShiftSharedLedButtonArray::applyLedState()
{
	// set the latchPin low so the LEDs don't change while you're sending in bits
	digitalWrite(_shiftLatchPin, LOW);
	// shift out the bits
	shiftOut(_shiftDataPin, _shiftClockPin, MSBFIRST, _ledState);
	// set the latch pin high so the LEDs will light up
	digitalWrite(_shiftLatchPin, HIGH);
}

int ShiftSharedLedButtonArray::getButtonDown()
{
	// if a button was recently triggered, return false
	if ((millis() - _lastTriggered) < kTriggerInterval)
	{
		return -1;
	}

	int result = -1;
	int readValue = analogRead(_readPin);

	for (int i = 0; i < _config.size(); i++)
	{
		if (readValue >= _config[i].readLow && readValue <= _config[i].readHigh)
		{
			result = i;
			_lastTriggered = millis();
		}
	}

	return result;
}
