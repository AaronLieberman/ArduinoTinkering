// LED sensor from http://playground.arduino.cc/Learning/LEDSensor

#include "LEDLightDetector.h"

#define LED_N_SIDE 2
#define LED_P_SIDE 3

#define LED_OUTPUT 7

LEDLightDetector g_detector(LED_P_SIDE, LED_N_SIDE, 100000);

void setup()
{
  pinMode(LED_OUTPUT, OUTPUT);
}

void loop()
{
  g_detector.loop();

  digitalWrite(LED_OUTPUT, g_detector.isActive() ? HIGH : LOW);

}

