#pragma once

class AnimationPlayer
{
public:
	AnimationPlayer(class ServoController& servoController);

	void initialize(const char* idleAnimationName);
	void update();

	void startAnimation(const char* animationName);
	bool isAnimationActive() const { return m_isAnimationActive; }

private:
	bool advanceFrame(bool rollOver);

	class ServoController& m_servoController;
	int m_idleAction = 0;
	int m_actionIndex = 0;
	int m_frameIndex = 0;
	int m_frameCount = 1;
	int m_lastFrameMillis = 0;
	bool m_isAnimationActive = false;
};
