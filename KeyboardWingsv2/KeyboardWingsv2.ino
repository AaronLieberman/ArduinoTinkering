#include "stlhelper.h"

#include "Debouncer.h"
#include "LatchButton.h"
#include "SerialPrintf.h"
#include "KeyScanner.h"

#include <Arduino.h>

// comment out the #pragma message in KeyboardLayouts in this library to reduce compile noise
#include <HID-Project.h>

#include <cmath>

#define WAIT_ON_SERIAL

const int kLedPin = LED_BUILTIN;

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
}

void loop() {
    _keyScanner.Scan();

    std::vector<std::string> debugKeys = _keyScanner.GetDebugKeys();

    for (const std::string &row : debugKeys) {
        Serial.println(row.c_str());
    }

    serialPrintfln();
    Serial.flush();

    static bool x_ledPin = false;
    digitalWrite(kLedPin, x_ledPin = !x_ledPin);

    delay(100);
}
