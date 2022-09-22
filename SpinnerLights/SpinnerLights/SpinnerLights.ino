#include <Arduino.h>

#include <Adafruit_ADXL345_U.h>
#include <Adafruit_NeoPixel.h>

#include "Color3F.h"
#include "SerialPrintf.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

#undef abs

const int kNeoPixelPin = 1;
const int kNumNeoPixels = 4;

Adafruit_NeoPixel _pixels(kNumNeoPixels, kNeoPixelPin, NEO_GRB + NEO_KHZ800);

Adafruit_ADXL345_Unified _accel(12345);
std::string _failedToInitializeReason;

const float kMinAccel = 12;
const float kMaxAccel = 40;

const int kAttractStartMillis = 0;
const int kAttractDurationMillis = 10000;
std::chrono::system_clock::time_point _lastActive = std::chrono::system_clock::now() - std::chrono::milliseconds(kAttractStartMillis);

//#define WAIT_ON_SERIAL

void setup() {
 	Serial.begin(115200);

	Serial.println("Opening serial");

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	Serial.println("Serial opened");

	_pixels.begin();

	if (!_accel.begin()) {
		_failedToInitializeReason = "Failed to find accelerometer";
		return;
	}

	Serial.println("Connected to accelerometer");

	_accel.setRange(ADXL345_RANGE_4_G); // 2, 4, 8 or 16 G

	_pixels.show();

	delay(10);
}

void failedToInitialize() {
	Serial.println(_failedToInitializeReason.c_str());

	for (int i = 0; i < kNumNeoPixels; i++) {
		_pixels.setPixelColor(i, 64, 0, 0);
	}

	_pixels.show();
	
	delay(1000);
}

float clamp(float v, float low, float high) {
	if (v > high) return high;
	if (v < low) return low;
	return v;
}

float mag(float x, float y, float z) {
	std::sqrt(x*x + y*y + z*z);
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

void showAxisColors(float x, float y, float z) {
	float xColor = clamp(std::abs(x), 0, 9.8) / 9.8f;
	float yColor = clamp(std::abs(y), 0, 9.8) / 9.8f;
	float zColor = clamp(std::abs(z), 0, 9.8) / 9.8f;

	setAllPixelColorsF(Color3F(xColor, yColor, zColor));
}

Color3F lerp(float v, Color3F a, Color3F b) {
	v = clamp(v, 0, 1);
	return a*v + b*(1-v);
}
 
void showSpin(float x, float y, float z) {
	float v = (clamp(-x, kMinAccel, kMaxAccel) - kMinAccel) / (kMaxAccel - kMinAccel);

	Color3F outer = Color3F(0.5, 1, 0);
	Color3F inner = Color3F(0.5, 1, 0);

	float flat = clamp(std::abs(z), 0, 9.8) / 9.8;
	Color3F flatColor = z > 0 ? Color3F(0, 0, 1) : Color3F(1, 0, 0);

	setPixelColorF(0, lerp(flat, flatColor, outer) * (v * v));
	setPixelColorF(1, lerp(flat, flatColor, inner) * (v * v));
	setPixelColorF(2, lerp(flat, flatColor, inner) * (v * v));
	setPixelColorF(3, lerp(flat, flatColor, outer) * (v * v));

	_pixels.show();
}

void showAttractMode(int millisSinceActive, int durationMillis) {
	if (millisSinceActive > durationMillis) {
		setAllPixelColorsF(Color3F(0.005));
	} else if (millisSinceActive > durationMillis - 1000) {
		// fade to lowest setting after duration
		Color3F a(0.005);
		Color3F b(0.06);
		float v = clamp((float)(millisSinceActive - (durationMillis - 1000)) / 1000, 0, 1);
		setAllPixelColorsF(lerp(v, a, b));
	} else {
		Color3F a(0.005);
		Color3F b(0.06);
		float v = (cos((float)millisSinceActive/1000 * M_PI) / 2) + 0.5;
		setAllPixelColorsF(lerp(v, a, b));
	}
}

void loop() {
	if (!_failedToInitializeReason.empty()) { failedToInitialize(); return; }

	sensors_event_t event;
	_accel.getEvent(&event);

	float x = event.acceleration.x;
	float y = event.acceleration.y;
	float z = event.acceleration.z;

	//showAxisColors(x, y, z);

	if (x < -kMinAccel) {
		showSpin(x, y, z);
		_lastActive = std::chrono::system_clock::now();
	} else {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		int millisSinceActive = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastActive).count();
		
		if (millisSinceActive > kAttractStartMillis) {
			showAttractMode(millisSinceActive - kAttractStartMillis, kAttractDurationMillis);
		} else {
			_pixels.clear();
			_pixels.show();
		}
	}
	
	delay(10);
}
