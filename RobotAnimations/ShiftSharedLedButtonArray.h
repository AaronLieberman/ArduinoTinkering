#pragma once

#include "stlhelper.h"
#include <vector>

struct ShiftSharedLedButtonConfig
{
	int readLow;
	int readHigh;
	int ledBit;
};

class ShiftSharedLedButtonArray
{
public:

	ShiftSharedLedButtonArray(int shiftLatchPin, int shiftClockPin, int shiftDataPin, int readPin,
		const std::vector<ShiftSharedLedButtonConfig>& config);

	void initialize();

	int getButtonDown();
	
	void clearLedState();
	void setLedState(int ledIndex, bool state);
	// call after clearing or setting LED state
	void applyLedState();

private:

	int _shiftLatchPin;
	int _shiftClockPin;
	int _shiftDataPin;
	int _readPin;
	std::vector<ShiftSharedLedButtonConfig> _config;

	int _buttonLatched = -1;
	int _lastTriggered = 0;
	
	int _ledState = 0;
	
};
