#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_CircuitPlayground.h>

#include "stlhelper.h"

#undef min
#undef max
#include <memory>
#include <vector>
#include <string>

#include "serialPrintf.h"
#include "Action.h"
#include "LatchButton.h"
#include "animationData.h"

using namespace std;

constexpr int kLightOnCountdownMsec = 500;
constexpr int kLoopDelay = 50;
constexpr int kOutputEnablePin = 9;
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
const vector<ServoConfig> kServoSpecs = { 
  ServoConfig(),
  { kServoKy66, 0, false}, { kServoKy66, 90, true}, { kServoKy66, 90, false}, { kServoA0090, 0, false},
  { kServoKy66, 0, false}, { kServoKy66, 90, true}, { kServoKy66, 90, false}, { kServoA0090, 0, false}
};
LatchButton leftButton(4, InputPinMode::PullDown);
LatchButton rightButton(5, InputPinMode::PullDown);

bool _outputEnable = true;
int _idleAction = 0;
int _actionIndex = 0;
int _frameIndex = 0;
int _frameCount = 1;
int _lastFrameMillis = 0;
  
void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(1);

  pinMode(kOutputEnablePin, OUTPUT);
  leftButton.initialize();
  rightButton.initialize();

  _servoMux.begin();
  _servoMux.setPWMFreq(60);

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
}

static void startAnimation(int actionIndex)
{
  _actionIndex = actionIndex;
  _frameIndex = 0;
  _frameCount = kActions[_actionIndex].frames.size();
  _lastFrameMillis = millis();
   
  serialPrintfln("starting animation %s (%d frames)", kActions[_actionIndex].name, kActions[_actionIndex].frames.size());
}

static void advanceFrame(bool rollOver)
{
  _frameIndex = rollOver
    ? (_frameIndex + 1) % _frameCount
    : _frameIndex + 1;
  _lastFrameMillis = millis();
}

void loop()
{
  if (leftButton.getAndClearState())
  {
    _outputEnable =  !_outputEnable;
    serialPrintfln("output enable -> %s", _outputEnable ? "true" : "false");
  }

  if (rightButton.getAndClearState())
  {
    startAnimation(2); // TODO hardcoded to Wave
  }

  if (_frameIndex >= _frameCount)
  {
    startAnimation(_idleAction);
  }
  else if (millis() - _lastFrameMillis > kMilliPerFrame)
  {
    advanceFrame(_actionIndex == _idleAction);
  }

  auto& frame = kActions[_actionIndex].frames[_frameIndex];

  for (int servoIndex = 0; servoIndex < kServoSpecs.size(); servoIndex++)
  {
    auto& servoConfig = kServoSpecs[servoIndex];
    auto& spec = servoConfig.spec;
    float angle = frame.boneAngles[servoIndex] + servoConfig.center;
    angle = servoConfig.flip ? (180 - angle) : angle;
    int pulseLen = (angle / 180) * (spec.pulseMax - spec.pulseMin) + spec.pulseMin;
    _servoMux.setPWM(servoIndex, 0, pulseLen);
  }

  CircuitPlayground.redLED(_outputEnable);
  digitalWrite(kOutputEnablePin, _outputEnable ? LOW : HIGH); // outputEnable is low when enabled

  delay(10);
}
