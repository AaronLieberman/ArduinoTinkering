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
#define NUMPIXELS      28

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

Color3F white(1, 1, 1);
Color3F black(0, 0, 0);
Color3F orange(1, 0.5f, 0);
Color3F yellow(1, 1, 0);
Color3F purple(1, 0, 1);
Color3F red(1, 0, 0);
Color3F blue(0, 0, 1);
Color3F green(0, 1, 0);

void setPixels(Color3F c, int from, int to)
{
  int s = 255;
  
  for (int i = from; i < to; i++)
  {
    pixels.setPixelColor(i, c.R * s, c.G * s, c.B * s);
  }

  pixels.show();
}

void loop()
{
  setPixels(yellow, 0, NUMPIXELS);
  delay(1000);
  setPixels(black, 0, NUMPIXELS);
  delay(100);
  setPixels(red, 0, NUMPIXELS);
  delay(100);
  setPixels(black, 0, NUMPIXELS);
  delay(100);
}

