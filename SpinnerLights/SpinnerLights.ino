#include <Arduino.h>

#include <Adafruit_LIS3DH.h>
#include <Adafruit_NeoPixel.h>

#include "Color3F.h"
#include "SerialPrintf.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

//#define CIRCUITPLAYGROUND

#ifdef CIRCUITPLAYGROUND
const int kNeoPixelPin = 3;
#else
const int kNeoPixelPin = 1;
#endif
const int kLedPin = 13;
const int kNumNeoPixels = 1;

Adafruit_NeoPixel _pixels(kNumNeoPixels, kNeoPixelPin, NEO_RGB + NEO_KHZ800);

#ifdef CIRCUITPLAYGROUND
Adafruit_NeoPixel _strip(5, 8, NEO_GRB + NEO_KHZ800); // 5 NeoPixels are on D8
#endif

Adafruit_LIS3DH _accel;

void setup() {
	Serial.begin(115200);

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	_pixels.begin();
#ifdef CIRCUITPLAYGROUND
	_strip.begin();
#endif

	if (!_accel.begin(0x18) && !_accel.begin(0x19)) {
		Serial.print("Failed to find accelerometer");
#ifdef CIRCUITPLAYGROUND
		_strip.fill(0xff0000);
		_strip.show();
#endif
		while (true) {
		}
	}

	_accel.setRange(LIS3DH_RANGE_2_G); // 2, 4, 8 or 16 G

	_pixels.show();
#ifdef CIRCUITPLAYGROUND
	_strip.show();
#endif

	pinMode(kLedPin, OUTPUT);

	delay(10);
}

uint8_t valueFromGravity(float v) {
	v = abs(v);
	v = v > 9.8 ? 9.8 : v;
	v = v / 9.8;
	return (uint8_t)(pow(v, 2) * 255);
}

void loop() {
	sensors_event_t event;
	_accel.getEvent(&event);

#if MODE == 1
	_pixels.setPixelColor(0, valueFromGravity(event.acceleration.x) << 16);
	_pixels.setPixelColor(1, valueFromGravity(event.acceleration.y) << 8);
	_pixels.setPixelColor(2, valueFromGravity(event.acceleration.z));
#else
	float yAccel = event.acceleration.y;
	uint8_t v = yAccel < -13 ? 255 : 0;
	for (int i = 0; i < kNumNeoPixels; i++) {
		_pixels.setPixelColor(i, v, v * 0.5f, 0);
	}
#endif

	_pixels.show();
	delay(10);
}
