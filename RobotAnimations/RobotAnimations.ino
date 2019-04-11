#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_CircuitPlayground.h>

#include "stlhelper.h"

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "serialPrintf.h"
#include "Action.h"
#include "LatchButton.h"
#include "PeriodicDebug.h"
#include "animationData.h"
#include "ShiftSharedLedButtonArray.h"

#define ENABLE_SERVOS

constexpr int kShiftLatchPin = A1; // 74HC595 ST_CP
constexpr int kShiftClockPin = A2; // 74HC595 SH_CP
constexpr int kShiftDataPin = A7; // 74HC595 DS
constexpr int kReadButtonsPin = A3;
constexpr int kOutputEnablePin = A6;

constexpr int kLightOnCountdownMsec = 500;
constexpr int kLoopDelay = 50;
constexpr int kFps = 30;
constexpr int kMilliPerFrame = 1000 / kFps;

#ifdef ENABLE_SERVOS
auto _servoMux = Adafruit_PWMServoDriver();
#endif

struct ServoSpec
{
  int pulseMin;
  int pulseMax;
};
const ServoSpec kServoA0090 = { 150, 650 };
const ServoSpec kServoKy66 = { 150, 520 };

struct ServoConfig
{
  ServoSpec spec;
  int center;
  bool flip;
};

// 150 (0) -> 400 (90) -> 650 (180)
//const vector<short> kServoCenter = { 0, 320, 320, 345, 290, 330, 320, 345, 290 };
const std::vector<ServoConfig> kServoSpecs = { 
  { { 0, 0 }, 0, false},
  { kServoKy66, 0, false}, { kServoKy66, 90, true}, { kServoKy66, 90, false}, { kServoA0090, 0, false},
  { kServoKy66, 0, false}, { kServoKy66, 90, true}, { kServoKy66, 90, false}, { kServoA0090, 0, false}
};
std::vector<int> _lastServoValues(kServoSpecs.size());
LatchButton _leftButton(CPLAY_LEFTBUTTON, InputPinMode::PullDown);
LatchButton _rightButton(CPLAY_RIGHTBUTTON, InputPinMode::PullDown);

const std::vector<ShiftSharedLedButtonConfig> kButtonSpecs = {
  { 992, 995, 4},
  { 1017, 1019, 5 },
  { 1023, 1023, 6 },
  { 943, 947, 7 },
};
ShiftSharedLedButtonArray _buttons(kShiftLatchPin, kShiftClockPin, kShiftDataPin, kReadButtonsPin, kButtonSpecs);

PeriodicDebug _periodicDebug(500, 0);

bool _outputEnable = true;
int _idleAction = 0;
int _actionIndex = 0;
int _frameIndex = 0;
int _frameCount = 1;
int _lastFrameMillis = 0;

void setup()
{
  CircuitPlayground.begin();

  Serial.begin(9600);

  Serial.println("Setting up pins");

  pinMode(kOutputEnablePin, OUTPUT);
  digitalWrite(kOutputEnablePin, HIGH); // outputEnable is low when enabled

  _periodicDebug.initialize();
  _leftButton.initialize();
  _rightButton.initialize();
  _buttons.initialize();

  Serial.println("Starting PWM");
  Serial.println("(if this doesn't continue, maybe the servo board is unplugged?)");

#if ENABLE_SERVOS
  _servoMux.begin();
  _servoMux.setPWMFreq(60);
  delay(10);
#endif

  Serial.println("Loading data");

  for (int i = 0; i < kActions.size(); i++)
  {
    if (strcmp(kActions[i].name, "Idle") == 0)
    {
      _idleAction = i;
      _actionIndex = i;
      startAnimation(i);
      break;
    }
  }

  Serial.println("Ready");
}

static void startAnimation(int actionIndex)
{
  _actionIndex = actionIndex;
  _frameIndex = 0;
  _frameCount = kActions[_actionIndex].frames.size();
  _lastFrameMillis = millis();
  
  serialPrintfln("starting animation %s (%d frames)", kActions[_actionIndex].name, kActions[_actionIndex].frames.size());
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
    _outputEnable =  !_outputEnable;
    serialPrintfln("output enable -> %s", _outputEnable ? "true" : "false");
  }

  if (_rightButton.getAndClearState())
  {
    startAnimation(2); // TODO hardcoded to Wave
  }

  if (millis() - _lastFrameMillis > kMilliPerFrame)
  {
    if (!advanceFrame(_actionIndex == _idleAction))
    {
      startAnimation(_idleAction);
    }
  }

  auto& frame = kActions[_actionIndex].frames[_frameIndex];

  for (int servoIndex = 0; servoIndex < kServoSpecs.size(); servoIndex++)
  {
    auto& servoConfig = kServoSpecs[servoIndex];
    auto& spec = servoConfig.spec;
    float angle = frame.boneAngles[servoIndex] + servoConfig.center;
    angle = servoConfig.flip ? (180 - angle) : angle;
    int pulseLen = (angle / 180) * (spec.pulseMax - spec.pulseMin) + spec.pulseMin;
    
    if (_lastServoValues[servoIndex] != pulseLen)
    {
      _lastServoValues[servoIndex] = pulseLen;
#ifdef ENABLE_SERVOS
      _servoMux.setPWM(servoIndex, 0, pulseLen);
#endif
    }
  }

  digitalWrite(kOutputEnablePin, _outputEnable ? LOW : HIGH); // outputEnable is low when enabled
  delay(1);

  int buttonDownIndex = _buttons.getButtonDown();
  if (buttonDownIndex != -1)
  {
    serialPrintfln("button %d down", buttonDownIndex);

    _buttons.clearLedState();
    _buttons.setLedState(buttonDownIndex, true);
    _buttons.applyLedState();
  }
}
