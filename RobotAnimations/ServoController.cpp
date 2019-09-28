#include "ServoController.h"

#include <Adafruit_PWMServoDriver.h>

ServoController::ServoController(const std::vector<ServoConfig>& servoSpecs)
	: m_servoSpecs(servoSpecs)
	, m_servoMux(std::unique_ptr<Adafruit_PWMServoDriver>())
{
}

void ServoController::initialize()
{
	m_servoMux->begin();
	m_servoMux->setPWMFreq(60);

	m_lastServoValues.resize(m_servoSpecs.size());
}

void ServoController::setPosition(int servoIndex, float angle)
{
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
