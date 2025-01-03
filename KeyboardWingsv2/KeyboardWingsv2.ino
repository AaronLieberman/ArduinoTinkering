#include "stlhelper.h"

#include "Encoder.h"
#include "KeyActionTracker.h"
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

const bool kWaitOnSerial = false;
const bool kUseSerial = true;

const bool kTestAllKeysMode = false;
const bool kEnableKeys = true && !kTestAllKeysMode;
const bool kTimersEnabled = false;

const int kLedPin = LED_BUILTIN;

const int kActiveScanDelayMs = 1;
const int kInactiveScanDelayMs = 30;
const long kInactiveDelayMs = 4000;

const uint8_t kLeftCols = 7;
const uint8_t kLeftRowOffset = 8;
const uint8_t kRightCols = 9;
const uint8_t kRightRowOffset = 9;
const uint8_t kRows = 6;

KeyScanner _keyScannerLeft(0x20, kLeftCols, kLeftRowOffset, kRows, 0);
KeyScanner _keyScannerRight(0x21, kRightCols, kRightRowOffset, kRows, kLeftCols);
long _lastActiveTime = 0;
int _keyDownCount = 0;

bool _ctrlKeyDown = false;

KeyboardLayout _layout;
KeyActionTracker _keyActionTracker;

const int kPlaySwitchPin = 3;
const int kEncoderButtonPin = 0;
const int kEncoderPinA = 1;
const int kEncoderPinB = 2;

Encoder _encoder(kEncoderPinA, kEncoderPinB);
LatchButton _playSwitch(kPlaySwitchPin);
LatchButton _encoderButton(kEncoderButtonPin);

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

void VolumeValueChanged(int change) {
    if (_ctrlKeyDown) {
        if (kUseSerial) {
            Serial.println(change > 0 ? "Next track" : "Previous track");
        }
        if (kEnableKeys) {
            Consumer.write(change > 0 ? MEDIA_NEXT : MEDIA_PREV);
        }
    } else {
        for (int i = 0; i < abs(change); i++) {
            if (kUseSerial) {
                Serial.println(change > 0 ? "Volume up" : "Volume down");
            }
            if (kEnableKeys) {
                Consumer.write(change > 0 ? MEDIA_VOLUME_UP : MEDIA_VOLUME_DOWN);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    SimpleTimer::SetEnabled(kTimersEnabled);

    while (kWaitOnSerial && !Serial) delay(100);

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
        Consumer.begin();
    }

    Serial.println("Starting IO left expander");
    Serial.flush();
    _keyScannerLeft.Init();
    Serial.println("Starting IO right expander");
    Serial.flush();
    _keyScannerRight.Init();

    _playSwitch.initialize();
    _encoderButton.initialize();

    _encoder.initialize();
    _encoder.setValueChanged([](int change) { VolumeValueChanged(change); });

    Serial.println("Setup complete");
    Serial.flush();

    if (kEnableKeys) {
        BootKeyboard.releaseAll();
    }
}

void PrintDebug() {
    if (!kUseSerial) {
        return;
    }

    static std::vector<std::string> debugKeysLeft, debugKeysSeenLeft, debugKeysRight, debugKeysSeenRight;
    _keyScannerLeft.GetDebugKeys(debugKeysLeft, debugKeysSeenLeft);
    _keyScannerRight.GetDebugKeys(debugKeysRight, debugKeysSeenRight);
    for (int rowIndex = 0; rowIndex < kRows; rowIndex++) {
        serialPrintfln("%s %s", (kTestAllKeysMode ? debugKeysSeenLeft : debugKeysLeft)[rowIndex].c_str(),
            (kTestAllKeysMode ? debugKeysSeenRight : debugKeysRight)[rowIndex].c_str());
    }

    serialPrintfln();
    Serial.flush();
}

bool ProcessSide(KeyScanner& keyScanner, bool fastScanResult) {
    static std::vector<std::pair<int, int>> keysDown, keysUp;
    bool changed = keyScanner.Scan(fastScanResult, keysDown, keysUp);

    if (changed) {
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

            _keyActionTracker.KeyDown(key);

            // handle special key combos
            if (_keyDownCount == 1) {
                // press the application key 3 times quickly and you get a win+L
                if (_keyActionTracker.GetKeyStatus({ KEY_APPLICATION }, 3)) {
                    if (kEnableKeys) {
                        BootKeyboard.press(KEY_LEFT_GUI);
                        BootKeyboard.write(KEY_L);
                        BootKeyboard.release(KEY_LEFT_GUI);
                    }
                }
            }
        }

        for (auto [row, col] : keysUp) {
            LayoutKey key = _layout.getKey(row, col);

            _keyActionTracker.KeyUp(key);

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

        _ctrlKeyDown = _keyActionTracker.GetKeyStatus({ KEY_LEFT_CTRL }, 1) ||
            _keyActionTracker.GetKeyStatus({ KEY_RIGHT_CTRL }, 1);
    }

    return changed;
}

void loop() {
    long now = millis();

    static SimpleTimer x_timerFastScan("fastscan", 100);  // <2ms
    x_timerFastScan.Start();
    bool fastScanResultLeft = _keyScannerLeft.FastScan();
    bool fastScanResultRight = _keyScannerRight.FastScan();
    x_timerFastScan.Stop();

    static SimpleTimer x_timerProcessSide("processSide", 100);  // 5ms per side active
    x_timerProcessSide.Start();
    bool changed = false;
    if (ProcessSide(_keyScannerLeft, fastScanResultLeft)) {
        changed = true;
    }

    if (ProcessSide(_keyScannerRight, fastScanResultRight)) {
        changed = true;
    }
    x_timerProcessSide.Stop();

    if (changed) {
        PrintDebug();
    } else if (!fastScanResultLeft && !fastScanResultRight && _keyDownCount != 0) {
        PrintDebug();

        _keyActionTracker.Reset();

        if (kEnableKeys) {
            BootKeyboard.releaseAll();
        }

        _keyDownCount = 0;
    }

    _encoder.update();

    if (_playSwitch.getAndClearState()) {
        if (kUseSerial) {
            Serial.println("Play/Pause");
        }
        if (kEnableKeys) {
            Consumer.write(MEDIA_PLAY_PAUSE);
        }
    }

    if (_encoderButton.getAndClearState()) {
        if (kUseSerial) {
            Serial.println("Mute");
        }
        if (kEnableKeys) {
            Consumer.write(MEDIA_VOLUME_MUTE);
        }
    }

    if (fastScanResultLeft || fastScanResultRight) {
        _lastActiveTime = now;
    }

    bool active = now <= _lastActiveTime + kInactiveDelayMs;
    digitalWrite(kLedPin, active ? LOW : HIGH);

    static bool lastActive = false;
    if (lastActive != active) {
        Serial.println(active ? "active" : "inactive");
        lastActive = active;
    }

    delay(active ? kActiveScanDelayMs : kInactiveScanDelayMs);
}
