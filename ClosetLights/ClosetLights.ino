#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "Color3F.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
const int neopixelPin = 6;

const int inputPin = 3;

// How many NeoPixels are attached to the Arduino?
const int numPixels = 28;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numPixels, neopixelPin, NEO_GRB + NEO_KHZ800);

int delayval = 1;
bool lightsOn = false;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(inputPin, INPUT_PULLUP);

  pixels.begin(); // This initializes the NeoPixel library.
}

void loop()
{
  lightsOn = digitalRead(inputPin) == HIGH;

  digitalWrite(LED_BUILTIN, lightsOn ? HIGH : LOW);
  
  for (int i = 0; i < numPixels; i++)
  {
    Color3F c = lightsOn ? Color3F(1, 1, 1) : Color3F(0, 0, 0);

    float b = 255;
    pixels.setPixelColor(i, pixels.Color(c.R * b, c.G * b, c.B * b  ));
    pixels.show(); // This sends the updated pixel color to the hardware.
  }

  delay(100);
}

