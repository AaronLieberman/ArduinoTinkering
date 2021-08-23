#include <Arduino.h>

#include "SharedButtonArray.h"

SharedButtonArray::SharedButtonArray(
    int readPin, int inputMode, const std::vector<SharedButtonConfig> &buttonConfig, int minTriggerInterval)
    : _readPin(readPin), _inputMode(inputMode), _buttonConfig(buttonConfig), _minTriggerInterval(minTriggerInterval) {}

void SharedButtonArray::initialize() {
	pinMode(_readPin, _inputMode);
}

uint SharedButtonArray::getMultipleButtonDown() {
	int readValue = analogRead(_readPin);
	uint result = 0;

	for (int i = 0; i < _buttonConfig.size(); i++) {
		if (readValue >= _buttonConfig[i].readTarget - _buttonConfig[i].tolerance &&
		    readValue <= _buttonConfig[i].readTarget + _buttonConfig[i].tolerance) {
			result += 1 << i;
		}
	}

	return result;
}

int SharedButtonArray::getButtonDown() {
	// if a button was recently triggered, return false
	if ((millis() - _lastTriggered) < _minTriggerInterval) {
		return -1;
	}

	int result = -1;
	int readValue = analogRead(_readPin);

	for (int i = 0; i < _buttonConfig.size(); i++) {
		if (readValue >= _buttonConfig[i].readTarget - _buttonConfig[i].tolerance &&
		    readValue <= _buttonConfig[i].readTarget + _buttonConfig[i].tolerance) {
			result = i;
			_lastTriggered = millis();
		}
	}

	return result;
}
