#include "stlhelper.h"

#include "KeyScanner.h"
#include "KeyboardLayout.h"
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

const bool kTestAllKeysMode = true;
const bool kEnableKeys = false && !kTestAllKeysMode;
const bool kTimersEnabled = false;

const int kLedPin = LED_BUILTIN;

const int kActiveScanDelayMs = 1;  // TODO reduce to 1
const int kInactiveScanDelayMs = 30;  // TODO reduce to 30
const long kInactiveDelayMs = 2000;

const uint8_t kLeftCols = 7;
const uint8_t kLeftRowOffset = 8;
const uint8_t kRightCols = 9;
const uint8_t kRightRowOffset = 9;
const uint8_t kRows = 6;

KeyScanner _keyScannerLeft(0x20, kLeftCols, kLeftRowOffset, kRows, 0);
KeyScanner _keyScannerRight(0x21, kRightCols, kRightRowOffset, kRows, kLeftCols);
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

    Serial.println("Starting IO left expander");
    Serial.flush();
    _keyScannerLeft.Init();
    Serial.println("Starting IO right expander");
    Serial.flush();
    _keyScannerRight.Init();

    Serial.println("Setup complete");
    Serial.flush();

    if (kEnableKeys) {
        BootKeyboard.releaseAll();
    }
}

bool Scan(KeyScanner &keyScanner) {
    // static just to avoid an extra allocation each loop. We clear at the start of keyscanner.Scan anyway
    static std::vector<std::pair<int, int>> keysDown;
    static std::vector<std::pair<int, int>> keysUp;

    static SimpleTimer x_timerScan("Scan", 100, kTimersEnabled);
    x_timerScan.Start();
    bool changed = keyScanner.Scan(keysDown, keysUp);
    x_timerScan.Stop();

    if (changed) {
        if (kUseSerial) {
            static std::vector<std::string> debugKeys, debugKeysSeen;
            keyScanner.GetDebugKeys(debugKeys, debugKeysSeen);
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
}

void loop() {
    long now = millis();

    static SimpleTimer x_timerFastScan("loop", 100, kTimersEnabled);
    x_timerFastScan.Start();
    bool fastScanResultLeft = _keyScannerLeft.FastScan();
    bool fastScanResultRight = _keyScannerRight.FastScan();
    x_timerFastScan.Stop();

    if (fastScanResultLeft || fastScanResultRight) {
        if (Scan(_keyScannerLeft)) {
            _lastActiveTime = now;
        }
        if (Scan(_keyScannerRight)) {
            _lastActiveTime = now;
        }
    } else if (_keyDownCount != 0) {
        if (kEnableKeys) {
            BootKeyboard.releaseAll();
        }
        _keyDownCount = 0;
    }

    bool active = now <= _lastActiveTime + kInactiveDelayMs;
    digitalWrite(kLedPin, active ? LOW : HIGH);

    delay(active ? kActiveScanDelayMs : kInactiveScanDelayMs);
}
