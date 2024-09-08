#include "stlhelper.h"

#include "Debouncer.h"
#include "KeyScanner.h"
#include "KeyboardLayout.h"
#include "LatchButton.h"
#include "SerialPrintf.h"
#include "SimpleTimer.h"

#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>
#include <Arduino.h>

// comment out the #pragma message in KeyboardLayouts in this library to reduce compile noise
#include <HID-Project.h>

#include <cmath>

#define WAIT_ON_SERIAL
const bool kUseSerial = true;

const int kLedPin = LED_BUILTIN;
const bool kTestAllKeysMode = true;
const bool kEnableKeys = false && !kTestAllKeysMode;

const int kInactiveScanDelayMs = 30;
const long kInactiveDelayMs = 2000;
const long kDebounceTimeMs = 0;

KeyScanner _keyScanner;
long _lastActiveTime = 0;
int _keyDownCount = 0;

KeyboardLayout _layout;

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

    Serial.println("Starting I2C");
    Wire.begin();
    Wire.setClock(400000);  // upgrade the clock because the devices we're using can support it

    pinMode(kLedPin, OUTPUT);
    digitalWrite(kLedPin, HIGH);

    Serial.println("Starting keyboard");
    Serial.flush();
    if (kEnableKeys) {
        BootKeyboard.begin();
    }

    Serial.println("Starting IO expander");
    Serial.flush();
    _keyScanner.Init();

    Serial.println("Setup complete");
    Serial.flush();

    Debouncer::setDebounceTime(kDebounceTimeMs);

    if (kEnableKeys) {
        BootKeyboard.releaseAll();
    }
}

void loop() {
    long now = millis();

    static std::vector<std::pair<int, int>> keysDown;
    static std::vector<std::pair<int, int>> keysUp;

    static SimpleTimer x_timerFastScan(100);
    x_timerFastScan.Start();
    KeyboardSide fastScanResult = _keyScanner.FastScan();
    x_timerFastScan.Stop();

    if (fastScanResult != KeyboardSide::None) {
        static SimpleTimer x_timerScan(100);
        x_timerScan.Start();
        bool changed = _keyScanner.Scan(keysDown, keysUp);
        x_timerScan.Stop();

        if (changed) {
            _lastActiveTime = now;

            if (kUseSerial) {
                static std::vector<std::string> debugKeys, debugKeysSeen;
                _keyScanner.GetDebugKeys(debugKeys, debugKeysSeen);
                for (const std::string& row : (kTestAllKeysMode ? debugKeysSeen : debugKeys)) {
                    Serial.println(row.c_str());
                }
            }

            for (auto [row, col] : keysDown) {
                LayoutKey key = _layout.getKey(row, col);
                if (key.keyboardKeycode != KEY_RESERVED) {
                    if (kEnableKeys) {
                        BootKeyboard.press(key.keyboardKeycode);
                    }
                    _keyDownCount++;
                } else if (key.consumerKeycode != 0) {
                    if (kEnableKeys) {
                        BootKeyboard.press(key.consumerKeycode);
                    }
                    _keyDownCount++;
                }
            }

            for (auto [row, col] : keysUp) {
                LayoutKey key = _layout.getKey(row, col);
                if (key.keyboardKeycode != KEY_RESERVED) {
                    if (kEnableKeys) {
                        BootKeyboard.release(key.keyboardKeycode);
                    }
                    _keyDownCount--;
                } else if (key.consumerKeycode != 0) {
                    if (kEnableKeys) {
                        BootKeyboard.release(key.consumerKeycode);
                    }
                    _keyDownCount--;
                }
            }

            if (kUseSerial) {
                serialPrintfln();
                Serial.flush();
            }
        }
    } else if (_keyDownCount != 0) {
        BootKeyboard.releaseAll();
        _keyDownCount = 0;
    }

    bool active = now <= _lastActiveTime + kInactiveDelayMs;
    digitalWrite(kLedPin, active ? LOW : HIGH);

    if (!active) {
        delay(kInactiveScanDelayMs);
    }
}
