#include <Arduino.h>

#include <Adafruit_LIS3DH.h>
#include <Adafruit_NeoPixel.h>

#include "AveragingFilter.h"
#include "Color3F.h"
#include "SerialPrintf.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

const int kNeoPixelPin = 14;
const int kLedPin = 13;
const int kNumNeoPixels = 3;

Adafruit_NeoPixel _pixels(kNumNeoPixels, kNeoPixelPin, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel _strip(5, 8, NEO_GRB + NEO_KHZ800); // 5 NeoPixels are on D8

Adafruit_LIS3DH _accel;

AveragingFilter _filterX(10);
AveragingFilter _filterY(10);
AveragingFilter _filterZ(10);

void setup() {
	Serial.begin(115200);

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	_pixels.begin();
	_strip.begin();

	if (!_accel.begin(0x18) && !_accel.begin(0x19)) {
		Serial.print("Failed to find accelerometer");
		_strip.fill(0xff0000);
		_strip.show();
		while (true) {
		}
	}

	_accel.setRange(LIS3DH_RANGE_4_G); // 2, 4, 8 or 16 G

	_pixels.show();
	_strip.show();

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

	_pixels.setPixelColor(0, valueFromGravity(_filterX.addSample(event.acceleration.x)) << 16);
	_pixels.setPixelColor(1, valueFromGravity(_filterY.addSample(event.acceleration.y)) << 8);
	_pixels.setPixelColor(2, valueFromGravity(_filterZ.addSample(event.acceleration.z)));

	_pixels.show();
	delay(10);
}
