#include <Arduino.h>

#include <Adafruit_ADXL345_U.h>
#include <Adafruit_NeoPixel.h>

#include "Color3F.h"
#include "SerialPrintf.h"

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#undef abs

const int kNeoPixelPin = 1;
const int kNumNeoPixels = 1;

Adafruit_NeoPixel _pixels(kNumNeoPixels, kNeoPixelPin, NEO_RGB + NEO_KHZ800);

const int kAttractStartMillis = 0;
const int kAttractDurationMillis = 10000;
std::chrono::system_clock::time_point _lastActive =
    std::chrono::system_clock::now() - std::chrono::milliseconds(kAttractStartMillis);

//#define WAIT_ON_SERIAL

void setup() {
	Serial.begin(9600);

	Serial.println("Opening serial");

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	Serial.println("Serial opened");

	_pixels.begin();

	_pixels.show();

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);

	delay(10);
}

float clamp(float v, float low, float high) {
	if (v > high)
		return high;
	if (v < low)
		return low;
	return v;
}

float mag(float x, float y, float z) {
	std::sqrt(x * x + y * y + z * z);
}

void setPixelColorF(int i, Color3F c) {
	_pixels.setPixelColor(i, c.R * 255, c.G * 255, c.B * 255);
}

void setAllPixelColorsF(Color3F c) {
	for (int i = 0; i < kNumNeoPixels; i++) {
		_pixels.setPixelColor(i, c.R * 255, c.G * 255, c.B * 255);
	}

	_pixels.show();
}

Color3F lerp(float v, Color3F a, Color3F b) {
	v = clamp(v, 0, 1);
	return a * v + b * (1 - v);
}

void showAttractMode(int millisSinceActive, int durationMillis) {
	Color3F a(1);
	Color3F b(0);
	float v = (cos((float)millisSinceActive / 1000 * M_PI) / 2) + 0.5;
	setAllPixelColorsF(lerp(v, a, b));
}

void loop() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	int millisSinceActive = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastActive).count();

	setAllPixelColorsF(Color3F(1, 1, 1));
	// showAttractMode(millisSinceActive - kAttractStartMillis, kAttractDurationMillis);

	digitalWrite(2, HIGH);
	digitalWrite(3, LOW);

	delay(100);
}
