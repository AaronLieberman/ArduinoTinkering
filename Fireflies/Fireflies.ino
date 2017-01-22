#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "Color3F.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
const int pinNeopixels = 5;
const int numNeopixels = 6;
const bool newPixelsAreRgb = false                                                                                                                                                                                                                  ;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
auto pixels = Adafruit_NeoPixel(numNeopixels, pinNeopixels, (newPixelsAreRgb ? NEO_RGB : NEO_GRB) + NEO_KHZ800);

struct Firefly
{
  Color3F color;
  bool active = false;
  int lifetime = 0;
  int progress = 0;
};

Firefly fireflies[numNeopixels];

int fireflyChance() { return 400; }
int fireflyLifetime() { return random (150, 250); }

Color3F fireflyColor()
{
  switch (random(0, 3))
  {
    case 0: return Color3F(102, 255, 64) / 255.f;
    case 1: return Color3F(32, 128, 48) / 255.f;
    case 2: return Color3F(255, 255, 32) / 255.f;
  }

  return Color3Fs::Blue();
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
  bool anyFirefliesActive = false;
  for (int i = 0; i < numNeopixels; i++)
  {
    anyFirefliesActive = anyFirefliesActive || fireflies[i].active;
  }

  int forceActivateIndex = anyFirefliesActive ? -1 : random(numNeopixels);

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
      if (forceActivateIndex == i || random(fireflyChance()) == 0)
      {
        firefly.active = true;
        firefly.lifetime = fireflyLifetime();
        firefly.progress = 0;
        firefly.color = fireflyColor();
      }
    }

    int s = sin((float)firefly.progress / firefly.lifetime * PI) * 255;
    auto color = firefly.active ? firefly.color * s : Color3Fs::Black();
    color = Color3Fs::White() * 255;

    pixels.setPixelColor(i, color.R, color.G, color.B);
  }

  pixels.show();

  delay(10);
}

