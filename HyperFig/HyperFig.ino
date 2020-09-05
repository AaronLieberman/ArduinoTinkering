#include "LatchButton.h"
#include "PeriodicDebug.h"
#include "SerialPrintf.h"
#include "SharedLedButtonArray.h"

#include "stlhelper.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

constexpr int kReadButtonsPin = A3;

const std::vector<SharedLedButtonConfig> kButtonSpecs = {
    {1017, 1019, 5},
    {992, 995, 4},
    {1023, 1023, 6},
    {943, 947, 7},
};
SharedLedButtonArray _buttons(kReadButtonsPin, kButtonSpecs);

PeriodicDebug _periodicDebug(500, 0);

int _activeButton = -1;

void setup() {
	Serial.begin(9600);

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
#ifdef PERIODIC_DEBUG
	_periodicDebug.update();
#endif

	delay(1);
}
