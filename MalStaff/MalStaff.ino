#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

#include "Color3F.h"
#include "LatchButton.h"

const int kNumNeoPixels = 10;

const int kSwitchPin = 4;

LatchButton switchButton(kSwitchPin);
bool powerOn = true;

void setup()
{
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(255);
  Serial.begin(115200);

  CircuitPlayground.setAccelTap(2, 120);
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), accelTapped, RISING);
  
	switchButton.initialize();
}

void accelTapped(void)
{
  //TODO: only toggle if pointed downwards?
  powerOn = !powerOn;
}

void loop()
{
	if (switchButton.getAndClearState())
	{
		powerOn = !powerOn;
	}

	if (powerOn)
	{
		for (int i = 0; i < kNumNeoPixels; i++)
		{
      CircuitPlayground.setPixelColor(i, 64, 0, 255);
		}
	}
	else
	{
		CircuitPlayground.clearPixels();
	}

  //auto z = CircuitPlayground.motionZ();
  //Serial.println(z); 
  //CircuitPlayground.setAccelTap(taps, threshhold)

  Serial.println(0);

  delay(10);
}
