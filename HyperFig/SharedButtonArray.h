#pragma once

#include "stlhelper.h"
#include <vector>

struct SharedButtonConfig {
	int readTarget;
	int tolerance;
};

class SharedButtonArray {
public:
	SharedButtonArray(
	    int readPin, int inputMode, const std::vector<SharedButtonConfig> &buttonConfig, int minTriggerInterval = 300);

	void initialize();

	int getButtonDown();
	uint getMultipleButtonDown();

private:
	int _readPin;
	int _inputMode;
	std::vector<SharedButtonConfig> _buttonConfig;
	int _minTriggerInterval;

	int _buttonLatched = -1;
	int _lastTriggered = 0;
};
