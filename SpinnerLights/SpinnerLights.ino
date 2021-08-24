#include <Arduino.h>

#include <Adafruit_LIS3DH.h>
#include <Adafruit_NeoPixel.h>

#include "Color3F.h"
#include "SerialPrintf.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

const int kNeoPixelPin = 14;
const int kLedPin = 13;
const int kNumNeoPixels = 3;

Adafruit_NeoPixel pixels(kNumNeoPixels, kNeoPixelPin, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip(5, 8, NEO_GRB + NEO_KHZ800); // 5 NeoPixels are on D8

Adafruit_LIS3DH accel;

void setup() {
	Serial.begin(115200);

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	pixels.begin();
	strip.begin();

	if (!accel.begin(0x18) && !accel.begin(0x19)) {
		Serial.print("Failed to find accelerometer");
		strip.fill(0xff0000);
		strip.show();
		while (true) {
		}
	}

	accel.setRange(LIS3DH_RANGE_4_G); // 2, 4, 8 or 16 G

	strip.show();

	pinMode(kLedPin, OUTPUT);

	pixels.show();

	delay(10);
}

uint8_t valueFromGravity(float v) {
	v = abs(v);
	v = v > 9.8 ? 9.8 : v;
	v = v / 9.8;
	return (uint8_t)(pow(v, 4) * 255);
}

void loop() {
	sensors_event_t event;
	accel.getEvent(&event);

	pixels.setPixelColor(0, valueFromGravity(event.acceleration.x) << 16);
	pixels.setPixelColor(1, valueFromGravity(event.acceleration.y) << 8);
	pixels.setPixelColor(2, valueFromGravity(event.acceleration.z));

	pixels.show();
}
