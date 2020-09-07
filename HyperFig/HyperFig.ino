#include "LatchButton.h"
#include "SerialPrintf.h"
#include "SharedLedButtonArray.h"

#include "stlhelper.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

constexpr int kReadButtonsPin = A5;

const std::vector<SharedLedButtonConfig> kButtonSpecs = {
    {395, 10, 9},
    {229, 10, 10},
    {519, 10, 11},
    {691, 10, 12},
};
SharedLedButtonArray _buttons(kReadButtonsPin, INPUT_PULLUP, kButtonSpecs, 0);

void setup() {
	Serial.begin(115200);

#ifdef WAIT_ON_SERIAL
	while (!Serial) {
		delay(10);
	}
#endif

	Serial.println("Setting up pins");
	_buttons.initialize();

	delay(10);
}

void loop() {
	int buttonDownIndex = _buttons.getButtonDown();
	_buttons.setSingleLedOn(buttonDownIndex);

	int value = analogRead(kReadButtonsPin);
	Serial.println(value);

	delay(50);
}
