// See setup instructions in LEDLightDetector.h

#include "LEDLightDetector.h"

const int pinOutput = 7;

LEDLightDetector g_detector(3, 2, 100000);

void setup()
{
  pinMode(pinOutput, OUTPUT);
}

void loop()
{
  g_detector.tick();

  digitalWrite(pinOutput, g_detector.isActive() ? HIGH : LOW);
}

