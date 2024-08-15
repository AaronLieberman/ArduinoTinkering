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
long _lastActiveTime = 0;

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

    static std::vector<std::pair<int, int>> keysDown;
    static std::vector<std::pair<int, int>> keysUp;
    bool changed = _keyScanner.Scan(keysDown, keysUp);

    if (changed) {
        _lastActiveTime = now;

        static std::vector<std::string> debugKeys;
        _keyScanner.GetDebugKeys(debugKeys);
        for (const std::string& row : debugKeys) {
            Serial.println(row.c_str());
        }

        if (!keysDown.empty()) {
            Serial.print("down: ");
            for (auto [row, col] : keysDown) {
                serialPrintf("%d,%d ", row, col);
                BootKeyboard.press(KeyboardKeycode::KEY_L);
            }
            serialPrintfln();
        }

        if (!keysUp.empty()) {
            Serial.print("up: ");
            for (auto [row, col] : keysUp) {
                serialPrintf("%d,%d ", row, col);
                BootKeyboard.release(KeyboardKeycode::KEY_L);
            }
            serialPrintfln();
        }

        serialPrintfln();
        Serial.flush();
    }

    bool active = now <= _lastActiveTime + kInactiveDelayMs;
    digitalWrite(kLedPin, active ? LOW : HIGH);

    if (!active) {
        delay(kInactiveScanDelayMs);
    }
}
