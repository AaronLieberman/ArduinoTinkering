#include "stlhelper.h"

#include "Debouncer.h"
#include "LatchButton.h"
#include "SerialPrintf.h"

#include <Adafruit_MCP23X17.h>

#include <cmath>

#define TEST_SERIAL
#define WAIT_ON_SERIAL

const int kLedPin = LED_BUILTIN;
bool _initSuccess = true;

Adafruit_MCP23X17 _ioPins;

#ifdef TEST_SERIAL
bool VerifyIoCall(bool result, const char* func, int line)
{
    if (!result) {
        serialPrintfln("IO failed at %s#%d", func, line);
        while (true) { delay(500); }
    }
    return result;
}

#define VERIFYIOCALL(result) VerifyIoCall(result, __func__, __LINE__)
#else
bool VerifyIoCall(bool result)
{
    if (!result) {
        while (true) { delay(500); }
    }
    return result;
}

#define VERIFYIOCALL(result)  VerifyIoCall(result)
#endif

void setup() {
#ifdef TEST_SERIAL
    Serial.begin(115200);
#endif

#ifdef WAIT_ON_SERIAL
    while (!Serial) { delay(100); }
#endif

    Serial.println("Serial connected");

    pinMode(kLedPin, OUTPUT);
    digitalWrite(kLedPin, HIGH);

    Serial.println("Starting IO expander");
    Serial.flush();
    delay( 1000 );

    VERIFYIOCALL(_ioPins.begin_I2C(0x20));

    for (int i = 0; i < 8; i++)
    {
        _ioPins.pinMode(i, OUTPUT);
    }

    for (int i = 8; i < 16; i++)
    {
        _ioPins.pinMode(i, INPUT_PULLUP);
    }
    
    _ioPins.writeGPIOA(255);

    Serial.println("Setup complete");
}

void loop() {
    uint8_t pinValues = _ioPins.readGPIOB();
//    uint8_t pinValues = 3;

#ifdef TEST_SERIAL
    serialPrintf("%d: ", pinValues);
    for (int i = 7; i >= 0; i--)
    {
        uint8_t pinValue = (pinValues >> i) & 1;
        serialPrintf("%d ", pinValue);
    }

    serialPrintfln();
#endif

    static bool x_ledPin = false;
    digitalWrite(kLedPin, x_ledPin = !x_ledPin);

    _ioPins.digitalWrite(3, x_ledPin); // Col3: S, W
    _ioPins.digitalWrite(2, !x_ledPin); // Col2: A
    //_ioPins.digitalWrite(2, LOW); // Col2: A
    //_ioPins.digitalWrite(3, LOW); // Col3: S, W

    delay(500);
}
