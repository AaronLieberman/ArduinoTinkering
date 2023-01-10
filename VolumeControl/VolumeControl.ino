#include "stlhelper.h"

// comment out the #pragma message in KeyboardLayouts in this library to reduce compile noise
#include "HID-Project.h"

#include "Encoder.h"
#include "LatchButton.h"

#include <cmath>

//#define TEST_SERIAL

const int kEncoderButtonPin = 0;
const int kPlaySwitchPin = 3;
const int kPrevSwitchPin = 4;
const int kNextSwitchPin = 5;
const int kLedPin = LED_BUILTIN;

const int kEncoderPinA = 1;
const int kEncoderPinB = 2;

Encoder _encoder(kEncoderPinA, kEncoderPinB, 2);
LatchButton _playSwitch(kPlaySwitchPin);
LatchButton _prevSwitch(kPrevSwitchPin);
LatchButton _nextSwitch(kNextSwitchPin);
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

    _playSwitch.initialize();
    _prevSwitch.initialize();
    _nextSwitch.initialize();
    _encoderButton.initialize();

    _encoder.initialize();
    _encoder.setValueChanged([](int change) { valueChanged(change); });

    Consumer.begin();
}

void loop() {
    bool anyPinDown = digitalRead(kPlaySwitchPin) == LOW || digitalRead(kEncoderButtonPin) == LOW;
    digitalWrite(kLedPin, anyPinDown ? HIGH : LOW);

    _encoder.update();

    if (_playSwitch.getAndClearState()) {
        Consumer.write(MEDIA_PLAY_PAUSE);
        delay(200);
    }
    
    if (_prevSwitch.getAndClearState()) {
        Consumer.write(MEDIA_NEXT);
        delay(200);
    }

    if (_nextSwitch.getAndClearState()) {
        Consumer.write(MEDIA_PREV);
        delay(200);
    }

    if (_encoderButton.getAndClearState()) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        delay(200);
    }
}
