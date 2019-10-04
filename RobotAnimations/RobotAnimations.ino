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

//#define WAIT_ON_SERIAL
//#define SETUP_MODE
//#define PERIODIC_DEBUG

constexpr int kShiftLatchPin = A1; // 74HC595 ST_CP
constexpr int kShiftClockPin = A2; // 74HC595 SH_CP
constexpr int kShiftDataPin = A7;  // 74HC595 DS
constexpr int kReadButtonsPin = A3;
constexpr int kOutputEnablePin = A6;

constexpr int kDisplayChangeMillis = 1000;
constexpr int kIdleTimeoutMillis = 10000;
constexpr float kLedWheelSpeed = 0.1f;
constexpr float kLedWheelDistance = 1.5f;

LatchButton _leftButton(CPLAY_LEFTBUTTON, InputPinMode::PullDown);
LatchButton _rightButton(CPLAY_RIGHTBUTTON, InputPinMode::PullDown);

const std::vector<ShiftSharedLedButtonConfig> kButtonSpecs = {
	{1017, 1019, 5},
	{992, 995, 4},
	{1023, 1023, 6},
	{943, 947, 7},
};
ShiftSharedLedButtonArray _buttons(kShiftLatchPin, kShiftClockPin, kShiftDataPin, kReadButtonsPin, kButtonSpecs);

#ifdef SETUP_MODE
bool _setupMode = true;
#else
bool _setupMode = false;
#endif

PeriodicDebug _periodicDebug(500, 0);
ServoController _servoController(kServoSpecs);
AnimationPlayer _animationPlayer(_servoController);

bool _outputEnable = true;
int _activeButton = -1;
int _lastDisplayChangeTime = 0;
int _idleTime = 0;

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

	randomSeed(analogRead(0));

	Serial.println("Loading data");

	_animationPlayer.initialize("Idle");

	Serial.println("Ready");

	_lastDisplayChangeTime = millis();
	_idleTime = millis();
}

void loop()
{
#ifdef PERIODIC_DEBUG
	_periodicDebug.update();
#endif

	if (_leftButton.getAndClearState())
	{
		_outputEnable = !_outputEnable;
		serialPrintfln("output enable -> %s", _outputEnable ? "true" : "false");
	}

	digitalWrite(kOutputEnablePin, _outputEnable && millis() < _idleTime + 1000 ? LOW : HIGH); // outputEnable is low when enabled

	if (_animationPlayer.isAnimationActive())
	{
		_idleTime = millis();
	}

	if (!_outputEnable)
	{
		CircuitPlayground.clearPixels();
		CircuitPlayground.setPixelColor(0, 255, 0, 0);
	}
	else if (_animationPlayer.isAnimationActive())
	{
		CircuitPlayground.clearPixels();
		static float x_pixelIndex = 0;
		x_pixelIndex += kLedWheelSpeed;
		if (x_pixelIndex > 10)
		{
			x_pixelIndex -= 10;
		}

		for (int i = 0; i < 10; i++)
		{
			float s1 = 1.0f - std::min(abs(x_pixelIndex - i) / kLedWheelDistance, 1.0f);
			float s2 = 1.0f - std::min(abs(x_pixelIndex - (i + 10)) / kLedWheelDistance, 1.0f);
			float s = s1 + s2;
			CircuitPlayground.setPixelColor(i, uint8_t(255 * s), uint8_t(255 * s), uint8_t(255 * s));
		}
	}
	else if (millis() < _idleTime + kIdleTimeoutMillis)
	{
		if (millis() > _lastDisplayChangeTime + kDisplayChangeMillis)
		{
			_lastDisplayChangeTime = millis();

			CircuitPlayground.clearPixels();
			uint8_t b = 32;
			for (int i = 0; i < 10; i++)
			{
				int r = random(1000);
				if (r < 5)
				{
					CircuitPlayground.setPixelColor(i, 0, 0, b);
				}
				else if (r < 40)
				{
					CircuitPlayground.setPixelColor(i, b, 0, 0);
				}
				else if (r < 850)
				{
					CircuitPlayground.setPixelColor(i, b, b, b);
				}
			}
		}
	}
	else
	{
		CircuitPlayground.clearPixels();
		CircuitPlayground.setPixelColor(0, 0, 32, 0);
	}

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
				case 1: _animationPlayer.startAnimation("Drums"); break;
				case 2: _animationPlayer.startAnimation("Robot"); break;
				case 3: _animationPlayer.startAnimation("Macarena"); break;
			}

			_activeButton = buttonDownIndex;
		}
	}

	delay(1);
}
