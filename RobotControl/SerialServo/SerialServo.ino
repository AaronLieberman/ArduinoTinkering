#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

constexpr int kLightOnCountdownMsec = 500;
constexpr int kLoopDelay = 50;

auto _servoMux = Adafruit_PWMServoDriver();
int _lightOnCountdown = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(1);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(9, OUTPUT);

  _servoMux.begin();
  _servoMux.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop()
{
  digitalWrite(LED_BUILTIN, _lightOnCountdown > 0 ? HIGH : LOW);
  _lightOnCountdown = _lightOnCountdown > 0 ? _lightOnCountdown - kLoopDelay : 0;

  while (true)
  {
    auto s = Serial.readStringUntil('\n');
    s.trim();
    if (s.length() == 0)
    {
      break;
    }

    _lightOnCountdown = kLightOnCountdownMsec;

    if (s.startsWith("servo "))
    {
      s = s.substring(6);
      
      int colonIndex = s.indexOf(':');
      if (colonIndex >= 0)
      {
        int servoIndex = s.toInt();
        String servoPosString = s.substring(colonIndex + 1);
        float servoPos = servoPosString.toFloat();
  
        float pulseLen = servoPos;
        _servoMux.setPWM(servoIndex, 0, pulseLen);
  
        String f("servo ");
        f.concat(servoIndex);
        f.concat(" -> ");
        f.concat(servoPos);
        Serial.println(f);
      }
    }
    
    if (s.startsWith("outputEnable: "))
    {
      s = s.substring(14);

      bool outputEnable = s.equals("true");
      // outputEnable is low when enabled
      digitalWrite(9, outputEnable ? LOW : HIGH);

      String f("output enable -> ");
      f.concat(outputEnable ? "true" : "false");
      Serial.println(f);
    }
  }

  delay(kLoopDelay);
}
