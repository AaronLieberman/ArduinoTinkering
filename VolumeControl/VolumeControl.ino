#include "stlhelper.h"

#include "HID-Project.h"

#include "Encoder.h"

#include <cmath>

//#define TEST_SERIAL

const int kEncoderButtonPin = 4;
const int kSwitchPin = 3;
const int kLedPin = LED_BUILTIN;

const int kEncoderPinA = 0;
const int kEncoderPinB = 1;

Encoder _encoder(kEncoderPinA, kEncoderPinB);

void valueChanged(int change) {
    for (int i = 0; i < std::abs(change); i++) {
#ifdef TEST_SERIAL
        Serial.println(change > 0 ? "Volume up" : "Volume down");
#endif
        Consumer.write(change > 0 ? MEDIA_VOLUME_UP : MEDIA_VOLUME_DOWN);
        delay(30);
    }
}

void setup() {
    pinMode(kEncoderButtonPin, INPUT_PULLUP);
    pinMode(kSwitchPin, INPUT_PULLUP);
    pinMode(kLedPin, OUTPUT);

    _encoder.initialize();
    attachInterrupt(
        digitalPinToInterrupt(kEncoderPinA), []() { _encoder.interruptATriggered(); }, CHANGE);
    attachInterrupt(
        digitalPinToInterrupt(kEncoderPinB), []() { _encoder.interruptBTriggered(); }, CHANGE);
    _encoder.setValueChanged([](int change) { valueChanged(change); });

    Consumer.begin();

#ifdef TEST_SERIAL
    Serial.begin(9600);
#endif
}

void loop() {
    bool anyPinDown = digitalRead(kSwitchPin) == LOW || digitalRead(kEncoderButtonPin) == LOW;
    digitalWrite(kLedPin, anyPinDown ? HIGH : LOW);

    _encoder.update();

    if (digitalRead(kSwitchPin) == LOW) {
        Consumer.write(MEDIA_PLAY_PAUSE);
        delay(200);
    }
}
