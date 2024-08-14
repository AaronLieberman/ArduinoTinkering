#include "stlhelper.h"

#include "Debouncer.h"
#include "KeyScanner.h"
#include "LatchButton.h"
#include "SerialPrintf.h"
#include "SimpleTimer.h"

#include <Arduino.h>

// comment out the #pragma message in KeyboardLayouts in this library to reduce compile noise
#include <HID-Project.h>

#include <cmath>

#define WAIT_ON_SERIAL

const int kLedPin = LED_BUILTIN;

const int kInactiveScanDelayMs = 10;
const long kInactiveDelayMs = 2000;
const long kDebounceTimeMs = 0;

KeyScanner _keyScanner;

#pragma region VerifyIoCall
bool VerifyIoCall(bool result, const char* func, int line) {
    if (!result) {
        serialPrintfln("IO failed at %s#%d", func, line);
        while (true) {
            delay(500);
        }
    }
    return result;
}

#define VERIFYIOCALL(result) VerifyIoCall(result, __func__, __LINE__)
#pragma endregion

void setup() {
    Serial.begin(115200);

#ifdef WAIT_ON_SERIAL
    while (!Serial) delay(100);
#endif

    Serial.println("Serial connected");

    pinMode(kLedPin, OUTPUT);
    digitalWrite(kLedPin, HIGH);

    Serial.println("Starting keyboard");
    Serial.flush();
    BootKeyboard.begin();

    Serial.println("Starting IO expander");
    Serial.flush();
    _keyScanner.Init();

    Serial.println("Setup complete");
    Serial.flush();

    Debouncer::setDebounceTime(kDebounceTimeMs);
}

void loop() {
    long now = millis();

    bool changed = _keyScanner.Scan();

    if (changed) {
        for (const std::string& row : _keyScanner.GetDebugKeys()) {
            Serial.println(row.c_str());
        }

        serialPrintfln();
        Serial.flush();
    }

    static long x_activeTime = 0;
    if (changed) {
        x_activeTime = now;
    }

    bool active = now <= x_activeTime + kInactiveDelayMs;
    digitalWrite(kLedPin, active ? LOW : HIGH);

    if (!active) {
        delay(kInactiveScanDelayMs);
    }
}
