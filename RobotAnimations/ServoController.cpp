#include "ServoController.h"

#include <Adafruit_PWMServoDriver.h>

#include <assert.h>

ServoController::ServoController(const std::vector<ServoConfig>& servoSpecs)
	: m_servoSpecs(servoSpecs)
	, m_servoMux(std::unique_ptr<Adafruit_PWMServoDriver>(new Adafruit_PWMServoDriver()))
{
}

void ServoController::initialize()
{
	Serial.println("ServoController: initialize (if this doesn't continue, maybe the servo board is unplugged?)");
	m_servoMux->begin();
	m_servoMux->setPWMFreq(60);

	m_lastServoValues.resize(m_servoSpecs.size());

	m_initialized = true;
	Serial.println("ServoController: initialize complete");
}

void ServoController::setEnabled(bool value)
{
	assert(m_initialized); // must call initialize() before calling other functions

	if (m_isEnabled == value)
		return;

	if (value)
	{
		m_servoMux->wakeup();
	}
	else
	{
		m_servoMux->sleep();
	}

	m_isEnabled = value;
}

void ServoController::setPosition(int servoIndex, float angle)
{
	assert(m_initialized); // must call initialize() before calling other functions

	auto& servoConfig = m_servoSpecs[servoIndex];
	auto& spec = servoConfig.spec;
	float centeredAngle = angle + servoConfig.center;
	float adjustedAngle = servoConfig.flip ? (180 - centeredAngle) : centeredAngle;
	int pulseLen = (adjustedAngle / 180) * (spec.pulseMax - spec.pulseMin) + spec.pulseMin;

	if (m_lastServoValues[servoIndex] != pulseLen)
	{
		m_lastServoValues[servoIndex] = pulseLen;
		m_servoMux->setPWM(servoIndex, 0, pulseLen);
	}
}
