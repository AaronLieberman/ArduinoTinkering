#include "AnimationPlayer.h"

#include <Arduino.h>

#include "ServoController.h"
#include "animationData.h"
#include "SerialPrintf.h"

constexpr int kFps = 30;
constexpr int kMilliPerFrame = 1000 / kFps;

AnimationPlayer::AnimationPlayer(ServoController& servoController)
	: m_servoController(servoController)
{
}

void AnimationPlayer::initialize(const char* idleAnimationName)
{
	startAnimation(idleAnimationName);
	m_idleAction = m_actionIndex;
	m_isAnimationActive = false; // force this back to false now that we've started the idle animation
}

void AnimationPlayer::update()
{
	if (millis() - m_lastFrameMillis > kMilliPerFrame)
	{
		if (!advanceFrame(m_actionIndex == m_idleAction))
		{
			startAnimation("Idle");
		}
	}

	if (m_actionIndex != -1)
	{
		auto& frame = kActions[m_actionIndex].frames[m_frameIndex];

		for (int servoIndex = 0; servoIndex < m_servoController.getServoCount(); servoIndex++)
		{
			m_servoController.setPosition(servoIndex, frame.boneAngles[servoIndex]);
		}
	}
}

void AnimationPlayer::startAnimation(const char* animationName)
{
	m_actionIndex = -1;

	for (int i = 0; i < kActionCount; i++)
	{
		if (strcmp(kActions[i].name, animationName) == 0)
		{
			m_actionIndex = i;
			break;
		}
	}

	m_frameIndex = 0;

	if (m_actionIndex != -1)
	{
		m_frameCount = kActions[m_actionIndex].frameCount;
		serialPrintfln(
			"starting animation %s (%d frames)", kActions[m_actionIndex].name, kActions[m_actionIndex].frameCount);
		m_isAnimationActive = m_actionIndex != m_idleAction;
	}
	else
	{
		serialPrintfln("couldn't find an animation named %s", animationName);
		m_isAnimationActive = false;
	}

	m_lastFrameMillis = millis();
}

bool AnimationPlayer::advanceFrame(bool rollOver)
{
	bool result = false;

	if (rollOver)
	{
		m_frameIndex = (m_frameIndex + 1) % m_frameCount;
		result = true;
	}
	else
	{
		if (m_frameIndex + 1 < m_frameCount)
		{
			m_frameIndex++;
			result = true;
		}
	}

	m_lastFrameMillis = millis();
	return result;
}
