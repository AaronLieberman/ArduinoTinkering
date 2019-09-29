#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_CircuitPlayground.h>

#include "stlhelper.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Action.h"
#include "LatchButton.h"
#include "PeriodicDebug.h"
#include "ServoController.h"
#include "ShiftSharedLedButtonArray.h"
#include "SerialPrintf.h"

#include "animationData.h"
#include "servoConfig.h"

constexpr int kShiftLatchPin = A1; // 74HC595 ST_CP
constexpr int kShiftClockPin = A2; // 74HC595 SH_CP
constexpr int kShiftDataPin = A7;  // 74HC595 DS
constexpr int kReadButtonsPin = A3;
constexpr int kOutputEnablePin = A6;

constexpr int kLightOnCountdownMsec = 500;
constexpr int kLoopDelay = 50;
constexpr int kFps = 30;
constexpr int kMilliPerFrame = 1000 / kFps;

ServoController _servoController(kServoSpecs);

LatchButton _leftButton(CPLAY_LEFTBUTTON, InputPinMode::PullDown);
LatchButton _rightButton(CPLAY_RIGHTBUTTON, InputPinMode::PullDown);

const std::vector<ShiftSharedLedButtonConfig> kButtonSpecs = {
	{992, 995, 4},
	{1017, 1019, 5},
	{1023, 1023, 6},
	{943, 947, 7},
};
ShiftSharedLedButtonArray _buttons(kShiftLatchPin, kShiftClockPin, kShiftDataPin, kReadButtonsPin, kButtonSpecs);

PeriodicDebug _periodicDebug(500, 0);

bool _outputEnable = false;
int _idleAction = 0;
int _actionIndex = 0;
int _frameIndex = 0;
int _frameCount = 1;
int _lastFrameMillis = 0;
int _activeButton = -1;

void setup()
{
	CircuitPlayground.begin();
	CircuitPlayground.clearPixels();

	Serial.begin(9600);
	
	while (!Serial); // ********

	Serial.println("Setting up pins");
	pinMode(kOutputEnablePin, OUTPUT);
	digitalWrite(kOutputEnablePin, HIGH); // outputEnable is low when enabled

	_periodicDebug.initialize();
	_leftButton.initialize();
	_rightButton.initialize();
	_buttons.initialize();

	_servoController.initialize();
	delay(10);

	Serial.println("Loading data");

	startAnimation("Idle");
	_idleAction = _actionIndex;

	Serial.println("Ready");
}

static void startAnimation(const char* animationName)
{
	_actionIndex = -1;

	for (int i = 0; i < kActions.size(); i++)
	{
		if (strcmp(kActions[i].name, animationName) == 0)
		{
			_actionIndex = i;
			break;
		}
	}

	_frameIndex = 0;

	if (_actionIndex != -1)
	{
		_frameCount = kActions[_actionIndex].frames.size();
		serialPrintfln(
			"starting animation %s (%d frames)", kActions[_actionIndex].name, kActions[_actionIndex].frames.size());
	}
	else
	{
		serialPrintfln("couldn't find an animation named %s", animationName);
	}

	_lastFrameMillis = millis();
}

static bool advanceFrame(bool rollOver)
{
	bool result = false;

	if (rollOver)
	{
		_frameIndex = (_frameIndex + 1) % _frameCount;
		result = true;
	}
	else
	{
		if (_frameIndex + 1 < _frameCount)
		{
			_frameIndex++;
			result = true;
		}
	}

	_lastFrameMillis = millis();
	return result;
}

void loop()
{
	_periodicDebug.update();

	if (_leftButton.getAndClearState())
	{
		_outputEnable = !_outputEnable;
		serialPrintfln("output enable -> %s", _outputEnable ? "true" : "false");
	}
	digitalWrite(kOutputEnablePin, _outputEnable ? LOW : HIGH); // outputEnable is low when enabled
	CircuitPlayground.setPixelColor(0, _outputEnable ? 0 : 255, _outputEnable ? 255 : 0, 0);

	if (millis() - _lastFrameMillis > kMilliPerFrame)
	{
		if (!advanceFrame(_actionIndex == _idleAction))
		{
			startAnimation("Idle");
			_activeButton = -1;
		}
	}

	if (_actionIndex != -1)
	{
		auto& frame = kActions[_actionIndex].frames[_frameIndex];

		for (int servoIndex = 0; servoIndex < _servoController.getServoCount(); servoIndex++)
		{
			_servoController.setPosition(servoIndex, frame.boneAngles[servoIndex]);
		}
	}

	int buttonDownIndex = _buttons.getButtonDown();
	if (buttonDownIndex != -1)
	{
		switch (buttonDownIndex)
		{
			case 0: startAnimation("Wave"); break;
			case 1: startAnimation("Dab"); break;
			case 2: startAnimation("Unknown1"); break;
			case 3: startAnimation("Unknown2"); break;
		}

		_activeButton = buttonDownIndex;
	}

	_buttons.clearLedState();
	if (_activeButton != -1)
	{
		_buttons.setLedState(_activeButton, true);
	}
	_buttons.applyLedState();

	delay(1);
}
