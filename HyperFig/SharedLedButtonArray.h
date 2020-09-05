#pragma once

#include "stlhelper.h"
#include <vector>

struct SharedLedButtonConfig {
	int readLow;
	int readHigh;
	int ledIndex;
};

class SharedLedButtonArray {
public:
	SharedLedButtonArray(int readPin, const std::vector<SharedLedButtonConfig> &config);

	void initialize();

	int getButtonDown();

	void clearLedState();
	void setLedState(int ledIndex, bool state);
	// call after clearing or setting LED state
	void applyLedState();

private:
	int _readPin;
	std::vector<SharedLedButtonConfig> _config;

	int _buttonLatched = -1;
	int _lastTriggered = 0;

	int _ledState = 0;
};
