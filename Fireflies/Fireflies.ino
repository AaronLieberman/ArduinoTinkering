#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "Color3F.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
const int pinNeopixels = 5;
const int numNeopixels = 5;
const bool newPixelsAreRgb = false;
const int pinNeopixels2 = 3;
const int numNeopixels2 = 1;
const bool newPixelsAreRgb2 = true;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
auto pixels = Adafruit_NeoPixel(numNeopixels, pinNeopixels, (newPixelsAreRgb ? NEO_RGB : NEO_GRB) + NEO_KHZ800);
auto pixels2 = Adafruit_NeoPixel(numNeopixels2, pinNeopixels2, (newPixelsAreRgb2 ? NEO_RGB : NEO_GRB) + NEO_KHZ800);

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

  pixels.begin();
  pixels2.begin();
}

void loop()
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

    pixels.setPixelColor(i, color.R, color.G, color.B);
  }

  pixels.show();
  auto glowScale = (sin((millis() % 2000) / 2000.f * 2 * PI) / 4 + 0.75);
  auto color2 = Color3F(255, 255, 64) * glowScale;
  pixels2.setPixelColor(0, color2.R, color2.G, color2.B);
  pixels2.show();

  delay(10);
}

