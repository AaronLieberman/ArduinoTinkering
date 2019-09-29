#pragma once

#include "stlhelper.h"
#include <vector>
#include <memory>

struct ServoSpec
{
	int pulseMin;
	int pulseMax;
};

struct ServoConfig
{
	ServoSpec spec;
	int center;
	bool flip;
};

class ServoController
{
public:
	ServoController(const std::vector<ServoConfig>& servoSpecs);

	void initialize();
	
	bool isEnabled() const { return m_isEnabled; }
	void setEnabled(bool value);

	int getServoCount() const { return m_servoSpecs.size(); }

	void setPosition(int servoIndex, float angle);

private:
	const std::vector<ServoConfig> m_servoSpecs;
	const std::unique_ptr<class Adafruit_PWMServoDriver> m_servoMux;

	bool m_initialized = false;
	bool m_isEnabled = true;
	std::vector<int> m_lastServoValues;
};
