#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

#include "stlhelper.h"

#include "Color3F.h"
#include "LatchButton.h"
#include "serialPrintf.h"
#include "AveragingFilter.h"

const int kNumNeoPixels = 10;

const int kSwitchPin = 4;

LatchButton switchButton(kSwitchPin);
bool powerOn = true;

AveragingFilter _filterX(10);
AveragingFilter _filterY(10);
AveragingFilter _filterZ(10);

void setup()
{
  CircuitPlayground.begin();
  Serial.begin(115200);

  CircuitPlayground.setAccelTap(2, 120);
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), accelTapped, RISING);
  
	switchButton.initialize();
}

void accelTapped(void)
{
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
    float brightness = CircuitPlayground.slideSwitch() ? 255 : 12;

    auto x = CircuitPlayground.motionX();
    auto y = CircuitPlayground.motionY();
    auto z = CircuitPlayground.motionZ();
    auto fx = _filterX.addSample(x);
    auto fy = _filterY.addSample(y);
    auto fz = _filterZ.addSample(z);

    //serialPrintfln("%f, %f, %f", x, y, z);
    //serialPrintfln("%f, %f, %f", fx, fy, fz);

    bool active = fz < 0 || (abs(fz) < 3 && sqrt(fx * fx + fy * fy) > 6);

    uint32_t color = 0;

    if (!active)
    {
      float glowScale = sin((float)millis() / 1000 * PI) * 0.3f + 0.7f;
      brightness *= glowScale;

      color = 0x4000ff;
    }
    else
    {
      float r = 0;
      float g = sin((float)millis() / 100 * 2 * PI) * 0.5f + 0.5f;
      float b = 0;
      
      color = (uint8_t)(r * 255) << 16 | (uint8_t)(g * 255) << 8 | (uint8_t)(b * 255);
    }

    CircuitPlayground.setBrightness(brightness);

		for (int i = 0; i < kNumNeoPixels; i++)
		{
      CircuitPlayground.setPixelColor(i, color);
		}
	}
	else
	{
		CircuitPlayground.clearPixels();
	}

  delay(10);
}
