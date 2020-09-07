#include <Arduino.h>

#include "SharedLedButtonArray.h"

SharedLedButtonArray::SharedLedButtonArray(
    int readPin, int inputMode, const std::vector<SharedLedButtonConfig> &buttonConfig, int minTriggerInterval)
    : _readPin(readPin), _inputMode(inputMode), _buttonConfig(buttonConfig), _minTriggerInterval(minTriggerInterval) {}

void SharedLedButtonArray::initialize() {
	pinMode(_readPin, _inputMode);

	for (int i = 0; i < _buttonConfig.size(); i++) {
		pinMode(_buttonConfig[i].ledPin, OUTPUT);
	}

	clearLedState();
}

void SharedLedButtonArray::clearLedState() {
	for (int i = 0; i < _buttonConfig.size(); i++) {
		digitalWrite(_buttonConfig[i].ledPin, LOW);
	}
}

void SharedLedButtonArray::setLedState(int ledIndex, bool state) {
	digitalWrite(_buttonConfig[ledIndex].ledPin, state ? HIGH : LOW);
}

void SharedLedButtonArray::setSingleLedOn(int ledIndex) {
	if (ledIndex == -1) {
		clearLedState();
	} else {
		for (int i = 0; i < _buttonConfig.size(); i++) {
			digitalWrite(_buttonConfig[i].ledPin, i == ledIndex ? HIGH : LOW);
		}
	}
}

int SharedLedButtonArray::getButtonDown() {
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
