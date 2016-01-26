// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "Color3F.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      5

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 1;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}

float hue = 0;

Color3F GenerateHue(float hue)
{
  float u = cos(hue * 2 * M_PI);
  float w = sin(hue * 2 * M_PI);

  Color3F ret;
  float s = 255;

  float third = 1.0f / 3;
  ret.R = (cos(hue * 2 * M_PI) / 2 + 0.5f) * s;
  ret.G = (cos((hue + third) * 2 * M_PI) / 2 + 0.5f) * s;
  ret.B = (cos((hue + 2 * third) * 2 * M_PI) / 2 + 0.5f) * s;

  return ret;
}

void loop()
{
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for(int i=0; i < NUMPIXELS; i++)
  {
    Color3F c = GenerateHue(hue + ((float)i) / NUMPIXELS);
    pixels.setPixelColor(i, pixels.Color(c.R, c.G, c.B));

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
  }

  hue += 0.01f;
  if (hue >= 1)
  {
    hue = 0;
  }
}

