#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

constexpr int kLightOnCountdownMsec = 500;
constexpr int kLoopDelay = 50;
constexpr int kOutputEnablePin = 9;

auto _servoMux = Adafruit_PWMServoDriver();

constexpr char* boneNames[] = {
  "Torso",
    "Clavicle.R",
    "Shoulder.R",
    "Upper.R",
    "Lower.R",
    "Clavicle.L",
    "Shoulder.L",
    "Upper.L",
    "Lower.L"
};

struct Bone
{
  byte angle;
};

struct Frame
{
  Bone bones[];
};

struct Action
{
  Frame frames[];
  int frameCount;
};

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(1);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(kOutputEnablePin, OUTPUT);

  _servoMux.begin();
  _servoMux.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  
}

void loop()
{
  

}
