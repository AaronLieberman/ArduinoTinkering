#include "stlhelper.h"

// comment out the #pragma message in KeyboardLayouts in this library to reduce compile noise
#include "HID-Project.h"

#include "Encoder.h"
#include "LatchButton.h"

#include <cmath>

//#define TEST_SERIAL

const int kEncoderButtonPin = 4;
const int kSwitchPin = 3;
const int kLedPin = LED_BUILTIN;

const int kEncoderPinA = 0;
const int kEncoderPinB = 1;

Encoder _encoder(kEncoderPinA, kEncoderPinB);
LatchButton _switch(kSwitchPin);
LatchButton _encoderButton(kEncoderButtonPin);

void valueChanged(int change) {
    for (int i = 0; i < std::abs(change); i++) {
#ifdef TEST_SERIAL
        Serial.println(change > 0 ? "Volume up" : "Volume down");
#endif
        Consumer.write(change > 0 ? MEDIA_VOLUME_UP : MEDIA_VOLUME_DOWN);
    }
}

void setup() {
#ifdef TEST_SERIAL
    Serial.begin(9600);
#endif

    pinMode(kLedPin, OUTPUT);

    _switch.initialize();
    _encoderButton.initialize();

    _encoder.initialize();
    _encoder.setValueChanged([](int change) { valueChanged(change); });

    Consumer.begin();
}

void loop() {
    bool anyPinDown = digitalRead(kSwitchPin) == LOW || digitalRead(kEncoderButtonPin) == LOW;
    digitalWrite(kLedPin, anyPinDown ? HIGH : LOW);

    _encoder.update();

    if (_switch.getAndClearState()) {
        Consumer.write(MEDIA_PLAY_PAUSE);
        delay(200);
    }

    if (_encoderButton.getAndClearState()) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        delay(200);
    }
}
