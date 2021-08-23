#include "LatchButton.h"
#include "SerialPrintf.h"
#include "SharedButtonArray.h"

#include "stlhelper.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

const int kReadButtonsPin = A5;
const int kLatchPin = 9;
const int kDataPin = 11;
const int kClockPin = 12;

const std::vector<SharedButtonConfig> kButtonSpecs = {
    {1015, 5},
    {1003, 5},
    {980, 10},
    {932, 15},
};
SharedButtonArray _buttons(kReadButtonsPin, INPUT_PULLUP, kButtonSpecs, 0);

void setup() {
	Serial.begin(115200);

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	Serial.println("Setting up pins");
	_buttons.initialize();

	pinMode(kLatchPin, OUTPUT);
	pinMode(kDataPin, OUTPUT);
	pinMode(kClockPin, OUTPUT);

	delay(10);
}

void loop() {
	// TODO: multiple buttons at once dont quite work
	uint buttonsDown = _buttons.getMultipleButtonDown();

	// int buttonDownIndex = _buttons.getButtonDown();
	// int value = 1 << buttonDownIndex;
	// serialPrintfln("%d - %d - %d", analogRead(kReadButtonsPin), buttonDownIndex, value);

	digitalWrite(kLatchPin, 0);
	shiftOut(kDataPin, kClockPin, MSBFIRST, buttonsDown);
	digitalWrite(kLatchPin, 1);

	delay(50);
}
