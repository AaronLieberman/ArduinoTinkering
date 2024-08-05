#include "stlhelper.h"

#include "Debouncer.h"
#include "LatchButton.h"
#include "SerialPrintf.h"

#include <Adafruit_MCP23X17.h>

#include <cmath>

#define WAIT_ON_SERIAL

const int kLedPin = LED_BUILTIN;
bool _initSuccess = true;

Adafruit_MCP23X17 _ioPinsLeft;
Adafruit_MCP23X17 _ioPinsRight;

const uint8_t kLeftCols = 7;
const uint8_t kLeftRowOffset = 8;
const uint8_t kLeftRows = 6;
const uint8_t kRightCols = 9;
const uint8_t kRightRowOffset = 9;
const uint8_t kRightRows = 6;

bool _leftEnabled = false;
bool _rightEnabled = false;

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

    Serial.println("Starting IO expander");
    Serial.flush();
    delay(1000);

    _leftEnabled = _ioPinsLeft.begin_I2C(0x20);
    if (!_leftEnabled) {
        Serial.println("Failed to connect to left side I2C");
    }
    _rightEnabled = _ioPinsRight.begin_I2C(0x21);
    if (!_rightEnabled) {
        Serial.println("Failed to connect to right side I2C");
    }

    // initially clear all pins to output
    for (int i = 0; i < 16; i++) {
        _ioPinsLeft.pinMode(i, OUTPUT);
        _ioPinsRight.pinMode(i, OUTPUT);
    }

    // left side uses 7 cols, 5 rows
    for (int i = 0; i < kLeftCols; i++) {
        _ioPinsLeft.pinMode(i, INPUT_PULLUP);
    }
    for (int i = kLeftRowOffset; i < kLeftRowOffset + kLeftRows; i++) {
        _ioPinsLeft.pinMode(i, OUTPUT);
    }

    // right side uses 9 cols, 6 rows
    for (int i = 0; i < kRightCols; i++) {
        _ioPinsRight.pinMode(i, INPUT_PULLUP);
    }

    for (int i = kRightRowOffset; i < kRightRowOffset + kRightRows; i++) {
        _ioPinsRight.pinMode(i, OUTPUT);
    }

    _ioPinsLeft.writeGPIOA(255);
    _ioPinsLeft.writeGPIOB(255);
    _ioPinsRight.writeGPIOA(255);
    _ioPinsRight.writeGPIOB(255);

    Serial.println("Setup complete");
}

void loop() {
    uint16_t pinValuesLeft = (_ioPinsLeft.readGPIOB() << 8) | _ioPinsLeft.readGPIOA();
    uint16_t pinValuesRight = (_ioPinsRight.readGPIOB() << 8) | _ioPinsRight.readGPIOA();

    char buf1[16];
    sprintf(buf1, "%02x", pinValuesLeft & 0x7f);

    serialPrintf("%s: ", buf1);
    for (int i = 0; i < kLeftCols; i++) {
        bool pinValue = ((pinValuesLeft >> i) & 1) > 0;
        serialPrintf("%s", pinValue ? "-" : "x");
    }

    serialPrintf(" ");

    for (int i = 0; i < kRightCols; i++) {
        bool pinValue = ((pinValuesRight >> i) & 1) > 0;
        serialPrintf("%s", pinValue ? "-" : "x");
    }

    serialPrintfln();

    static bool x_ledPin = false;
    digitalWrite(kLedPin, x_ledPin = !x_ledPin);

    _ioPinsLeft.digitalWrite(9, x_ledPin);  // Row1
    _ioPinsLeft.digitalWrite(10, !x_ledPin);  // Row2

    delay(500);
}
