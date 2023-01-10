#include "stlhelper.h"

#include "Debouncer.h"
#include "LatchButton.h"
#include "SerialPrintf.h"

#include <TCA9555.h>

#include <cmath>

#define TEST_SERIAL
// #define WAIT_ON_SERIAL

const int kLedPin = LED_BUILTIN;
const int kSwitchPin = 0;

LatchButton _switch(kSwitchPin);

TCA9555 _ioPins(0x20);

int _counter = 1;

#ifdef TEST_SERIAL
#define VERIFYIOCALL(result)                                                                     \
    if (!result) {                                                                               \
        serialPrintfln("IO failed at %s#%s, code: %d", __func__, __LINE__, _ioPins.lastError()); \
    }
#else
#define VERIFYIOCALL(result) result
#endif

void setup() {
#ifdef TEST_SERIAL
    Serial.begin(115200);
#endif

#ifdef WAIT_ON_SERIAL
    while (!Serial) {
    }
#endif

    Serial.println("Serial connected");

    pinMode(kLedPin, OUTPUT);
    _switch.initialize();

    Serial.println("Starting IO expander");
    VERIFYIOCALL(_ioPins.begin());

    Serial.println("Updating pins");
    updatePins();

    Serial.println("Setup complete");
}

void updatePins() {
    int bits = _counter << 8;
    VERIFYIOCALL(_ioPins.write16(bits))
}

void loop() {
    if (_switch.getAndClearState()) {
        _counter = (_counter + 1) % 16;
#ifdef TEST_SERIAL
        serialPrintfln("Counter now %d", _counter);
#endif
        updatePins();
    }

    delay(10);
}
