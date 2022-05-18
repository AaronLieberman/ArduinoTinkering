#include <Arduino.h>

#include <Adafruit_LIS3DH.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_NeoPixel.h>

#include "Color3F.h"
#include "SerialPrintf.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

const int kNeoPixelPin = 1;
const int kNumNeoPixels = 4;

Adafruit_NeoPixel _pixels(kNumNeoPixels, kNeoPixelPin, NEO_GRB + NEO_KHZ800);

Adafruit_LIS3DH _accel;
std::string failedToInitializeReason;

#define WAIT_ON_SERIAL

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

	if (!_accel.begin(0x53, 0x4a)) {
		failedToInitializeReason = "Failed to find accelerometer";
		return;
	}

	Serial.println("Connected to accelerometer");

	_accel.setRange(LIS3DH_RANGE_2_G); // 2, 4, 8 or 16 G

	_pixels.show();

	delay(10);
}

// uint8_t valueFromGravity(float v) {
// 	v = abs(v);
// 	v = v > 9.8 ? 9.8 : v;
// 	v = v / 9.8;
// 	return (uint8_t)(pow(v, 2) * 255);
// }
// _pixels.setPixelColor(0, valueFromGravity(event.acceleration.x) << 16);
// _pixels.setPixelColor(1, valueFromGravity(event.acceleration.y) << 8);
// _pixels.setPixelColor(2, valueFromGravity(event.acceleration.z));

void failedToInitialize() {
	Serial.println(failedToInitializeReason.c_str());

	for (int i = 0; i < kNumNeoPixels; i++) {
		_pixels.setPixelColor(i, 64, 0, 0);
	}

	_pixels.show();
	
	delay(1000);
}

void loop() {
	if (!failedToInitializeReason.empty()) { failedToInitialize(); return; }

	sensors_event_t event;
	_accel.getEvent(&event);

	const float targetAccel = 100;
	//float yAccel = event.acceleration.y;
	float yAccel = 0;
	uint8_t v = yAccel < targetAccel ? 255 : 0;

	for (int i = 0; i < kNumNeoPixels; i++) {
		_pixels.setPixelColor(i, v*0.5f, v * 0.25f, 0);
	}

	_pixels.show();

	serialPrintfln("%f, %f, %f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
	
	delay(100);
}
