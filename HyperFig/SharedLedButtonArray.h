#pragma once

#include "stlhelper.h"
#include <vector>

struct SharedLedButtonConfig {
	int readTarget;
	int tolerance;
	int ledPin;
};

class SharedLedButtonArray {
public:
	SharedLedButtonArray(int readPin, int inputMode, const std::vector<SharedLedButtonConfig> &buttonConfig,
	    int minTriggerInterval = 300);

	void initialize();

	int getButtonDown();

	void clearLedState();
	void setLedState(int ledIndex, bool state);
	void setSingleLedOn(int ledIndex);

private:
	int _readPin;
	int _inputMode;
	std::vector<SharedLedButtonConfig> _buttonConfig;
	int _minTriggerInterval;

	int _buttonLatched = -1;
	int _lastTriggered = 0;
};
