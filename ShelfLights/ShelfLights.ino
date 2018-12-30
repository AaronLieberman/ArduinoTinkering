#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Color3F.h"
#include "LatchButton.h"

const int kNeoPixelPin = 4;
const int kNumNeoPixels = 15;

const int kSwitchPin = 2;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(kNumNeoPixels, kNeoPixelPin, NEO_GRB + NEO_KHZ800);

float hue = 0;
const float hueOffset = 0.002f;
float phase = 0;
const float phaseOffset = 1.0f / 167;
const float widthScale = 1.5f;

LatchButton switchButton(kSwitchPin);
bool powerOn = false;

void setup()
{
	pixels.begin();
	switchButton.initialize();
}

Color3F AnimateColor(Color3F baseColor, float ampScale, float x)
{
	float s = ((cos(x * 2 * M_PI) / 2 + 0.5f) * ampScale + (1.0f - ampScale));
	return baseColor * s;
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
			Color3F baseColor = Color3F::FromBytes(255, 199, 84);
			Color3F c = AnimateColor(baseColor, 0.8f, hue + ((float)i / widthScale) / kNumNeoPixels) * 0.6f;

			float brightness = cos((phase + 1.0f * i / kNumNeoPixels) * 2 * M_PI) / 2 + 0.5f * 255;
			pixels.setPixelColor(i, pixels.Color(c.R * brightness, c.G * brightness, c.B * brightness));
		}

		pixels.show();
		delay(1);
	}
	else
	{
		for (int i = 0; i < kNumNeoPixels; i++)
		{
			pixels.setPixelColor(i, pixels.Color(0, 0, 0));
		}

		pixels.show();
		delay(100);
	}

	hue = hue + hueOffset >= 1 ? 0 : hue + hueOffset;
	phase = phase + phaseOffset >= 1 ? 0 : phase + phaseOffset;
}
