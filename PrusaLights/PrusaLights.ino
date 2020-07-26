#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const int kLightsOnPin = 2;

const int kNeoPixelsPinA = 4;
const int kNumberOfPixelsA = 7;
const int kNeoPixelsPinB = 3;
const int kNumberOfPixelsB = 7;

Adafruit_NeoPixel pixelsA(kNumberOfPixelsA, kNeoPixelsPinA, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel pixelsB(kNumberOfPixelsB, kNeoPixelsPinB, NEO_RGBW + NEO_KHZ800);

void setup()
{
	Serial.begin(115200);

	pixelsA.begin();
	pixelsA.clear();
	pixelsB.begin();
	pixelsB.clear();

	pinMode(kLightsOnPin, INPUT);

	delay(500);
}

void loop()
{
	bool lightsOn = digitalRead(kLightsOnPin) == HIGH;

	const int kLightsOffValue = 0;
	const int kLightsOnValue = 255;

	if (lightsOn)
	{
		pixelsA.fill(0xffffffff, 0, kNumberOfPixelsA);
		pixelsB.fill(0xffffffff, 0, kNumberOfPixelsB);
	}
	else
	{
		pixelsA.clear();
		pixelsB.clear();
	}

	pixelsA.show();
	pixelsB.show();

	delay(10);
}
