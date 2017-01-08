#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "Color3F.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
const int pinNeopixels = 5;
const int numNeopixels = 10;
const int pinPot = A0;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
auto pixels = Adafruit_NeoPixel(numNeopixels, pinNeopixels, NEO_GRB + NEO_KHZ800);

struct Firefly
{
  Color3F color;
  bool active = false;
  int lifetime = 0;
  int progress = 0;
};

Firefly fireflies[numNeopixels];

int fireflyChance(float scaler) { return 1000 * scaler; }
int fireflyLifetime() { return random (100, 200); }

Color3F fireflyColor()
{
  switch (random(0, 3))
  {
    case 1: return Color3F(0.75f, 0.75f, 0.25f);
    case 2: return Color3F(0.125f, 0.5f, 0.125f);
  }

  return Color3F(1, 1, 1);
}

void setup()
{
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}

void setPixels(Color3F c, int from, int to, bool show, int brightness = 255)
{
  for (int i = from; i < to; i++)
  {
    pixels.setPixelColor(i, c.R * brightness, c.G * brightness, c.B * brightness);
  }

  if (show)
  {
    pixels.show();
  }
}

void loop()
{
  float potValue = (float)analogRead(pinPot) / 1023;

  for (int i = 0; i < numNeopixels; i++)
  {
    auto& firefly = fireflies[i];
    
    if (firefly.active)
    {
      firefly.progress++;

      if (firefly.progress >= firefly.lifetime)
      {
        firefly.active = false;
      }
    }
    else
    {
      if (random(0, fireflyChance(potValue)) == 0)
      {
        firefly.active = true;
        firefly.lifetime = fireflyLifetime();
        firefly.progress = 0;
        firefly.color = fireflyColor();
      }
    }

    int s = sin((float)firefly.progress / firefly.lifetime * PI) * 255;
    auto color = firefly.active ? firefly.color * s : Color3Fs::Black();

    pixels.setPixelColor(i, color.R, color.G, color.B);
  }

  pixels.show();

  delay(10);
}

