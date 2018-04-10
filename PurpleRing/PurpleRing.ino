#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "Color3F.h"

const int pinNeopixels = A5;
const int numNeopixels = 45;
const bool newPixelsAreRgb = false;
auto pixels = Adafruit_NeoPixel(numNeopixels, pinNeopixels, (newPixelsAreRgb ? NEO_RGB : NEO_GRB) + NEO_KHZ800);

void setup()
{
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin();
}

float s = 0;

void loop()
{
  s = 1;
  
  auto color = Color3F(127, 0, 255) * s;

  for (int i = 0; i < numNeopixels; i++)
  {
    pixels.setPixelColor(i, color.R, color.G, color.B);
  }

  pixels.show();
  delay(1);

  s = s <= 1 ? s + 0.0001f : 0;
}

