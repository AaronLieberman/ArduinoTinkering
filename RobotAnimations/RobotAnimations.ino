#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_CircuitPlayground.h>

#include "stlhelper.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "LatchButton.h"
#include "PeriodicDebug.h"
#include "ServoController.h"
#include "ShiftSharedLedButtonArray.h"
#include "SerialPrintf.h"
#include "AnimationPlayer.h"

#include "servoConfig.h"

#define WAIT_ON_SERIAL
#define SETUP_MODE

constexpr int kShiftLatchPin = A1; // 74HC595 ST_CP
constexpr int kShiftClockPin = A2; // 74HC595 SH_CP
constexpr int kShiftDataPin = A7;  // 74HC595 DS
constexpr int kReadButtonsPin = A3;
constexpr int kOutputEnablePin = A6;

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
ServoController _servoController(kServoSpecs);
AnimationPlayer _animationPlayer(_servoController);

bool _outputEnable = false;
int _activeButton = -1;

#ifdef SETUP_MODE
bool _setupMode = true;
#else
bool _setupMode = false;
#endif

void setup()
{
	CircuitPlayground.begin();
	CircuitPlayground.clearPixels();

	Serial.begin(9600);

#ifdef WAIT_ON_SERIAL
	while (!Serial)
	{
	}
#endif

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

	_animationPlayer.initialize("Idle");

	Serial.println("Ready");
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

	if (_setupMode)
	{
		static int x_servoIndex = -1;
		static int x_angle = 0;

		if (x_servoIndex == -1)
		{
			x_servoIndex = 0;
			_servoController.setPosition(x_servoIndex, x_angle);
		}

		int buttonDownIndex = _buttons.getButtonDown();
		if (buttonDownIndex != -1)
		{
			switch (buttonDownIndex)
			{
				case 0:
					x_servoIndex = std::min(x_servoIndex + 1, _servoController.getServoCount());
					x_angle = 0;
					break;
				case 1:
					x_servoIndex = std::max(x_servoIndex - 1, 0);
					x_angle = 0;
					break;
				case 2: x_angle += 10; break;
				case 3: x_angle -= 10; break;
			}

			_activeButton = buttonDownIndex;

			serialPrintfln("set servo %d pos -> %d", x_servoIndex, x_angle);
			_servoController.setPosition(x_servoIndex, x_angle);
		}
	}
	else
	{
		_animationPlayer.update();

		int buttonDownIndex = _buttons.getButtonDown();
		if (buttonDownIndex != -1)
		{
			switch (buttonDownIndex)
			{
				case 0: _animationPlayer.startAnimation("Wave"); break;
				case 1: _animationPlayer.startAnimation("Dab"); break;
				case 2: _animationPlayer.startAnimation("Unknown1"); break;
				case 3: _animationPlayer.startAnimation("Unknown2"); break;
			}

			_activeButton = buttonDownIndex;
		}
	}

	_buttons.clearLedState();
	if (_activeButton != -1)
	{
		_buttons.setLedState(_activeButton, true);
	}
	_buttons.applyLedState();

	delay(1);
}
