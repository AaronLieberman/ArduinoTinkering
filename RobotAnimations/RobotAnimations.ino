#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_CircuitPlayground.h>

#include "stlhelper.h"

#undef min
#undef max
#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "serialPrintf.h"
#include "Action.h"
#include "LatchButton.h"
#include "PeriodicDebug.h"
#include "animationData.h"

constexpr int kLightOnCountdownMsec = 500;
constexpr int kLoopDelay = 50;
constexpr int kOutputEnablePin = A2;
constexpr int kFps = 30;
constexpr int kMilliPerFrame = 1000 / kFps;

auto _servoMux = Adafruit_PWMServoDriver();

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
  ServoConfig(),
  { kServoKy66, 0, false}, { kServoKy66, 90, true}, { kServoKy66, 90, false}, { kServoA0090, 0, false},
  { kServoKy66, 0, false}, { kServoKy66, 90, true}, { kServoKy66, 90, false}, { kServoA0090, 0, false}
};
std::vector<int> _lastServoValues(kServoSpecs.size());
LatchButton _leftButton(CPLAY_LEFTBUTTON, InputPinMode::PullDown);
LatchButton _rightButton(CPLAY_RIGHTBUTTON, InputPinMode::PullDown);
PeriodicDebug _periodicDebug(500, 0);

bool _outputEnable = true;
int _idleAction = 0;
int _actionIndex = 0;
int _frameIndex = 0;
int _frameCount = 1;
int _lastFrameMillis = 0;

void setup()
{
  Serial.begin(9600);
  //while (!Serial) { }
  delay(200);

  Serial.println("Setting up pins");

  CircuitPlayground.begin();

  pinMode(kOutputEnablePin, OUTPUT);
  digitalWrite(kOutputEnablePin, HIGH); // outputEnable is low when enabled

  //_periodicDebug.initialize();
  _leftButton.initialize();
  _rightButton.initialize();

  Serial.println("Starting PWM");

  _servoMux.begin();
  _servoMux.setPWMFreq(60);
  delay(10);

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
      _servoMux.setPWM(servoIndex, 0, pulseLen);
    }
  }

  digitalWrite(kOutputEnablePin, _outputEnable ? LOW : HIGH); // outputEnable is low when enabled
  delay(1);
}
