//#include "Color3F.h"

#include "SerialPrintf.h"

//#include "stlhelper.h" // includes Arduino.h
#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

const int kNeoPixelPin = 1;
const int kLedPin = 13;
const int kNumNeoPixels = 3;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(kNumNeoPixels, kNeoPixelPin, NEO_RGB + NEO_KHZ800);

void setup() {
	Serial.begin(115200);

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	pinMode(kLedPin, OUTPUT);

	pixels.begin();
	pixels.show();

	delay(10);
}

void loop() {
	// digitalWrite(kLedPin, LOW);
	// delay(300);
	// digitalWrite(kLedPin, HIGH);
	// delay(150);

	static int foo = 0;
	foo++;

	for (int i = 0; i < kNumNeoPixels; i++) {
		int v = foo % kNumNeoPixels == i ? 255 : 0;
		pixels.setPixelColor(i, v, 0, 0);
	}
	pixels.show();

	digitalWrite(kLedPin, (foo % 2) == 0 ? LOW : HIGH);
	delay(100);
}
