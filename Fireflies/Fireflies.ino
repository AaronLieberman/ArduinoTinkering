#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "Color3F.h"

const int pinNeopixels = 5;
const bool newPixelsAreRgb = false;
auto pixels = Adafruit_NeoPixel(1, pinNeopixels, (newPixelsAreRgb ? NEO_RGB : NEO_GRB) + NEO_KHZ800);

const int fireflyPins[] = { 9, 10, 11 };
const int fireflyPinCount = sizeof(fireflyPins) / sizeof(fireflyPins[0]);

struct Firefly
{
  bool active = false;
  int lifetime = 0;
  int progress = 0;
};

Firefly fireflies[fireflyPinCount];

int fireflyChance() { return 400; }
int fireflyLifetime() { return random (150, 250); }

void setup()
{
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin();

  for (int i = 0; i < fireflyPinCount; i++)
  {
    pinMode(fireflyPins[i], OUTPUT);
  }
}

void loop()
{ 
  bool anyFirefliesActive = false;
  for (int i = 0; i < fireflyPinCount; i++)
  {
    anyFirefliesActive = anyFirefliesActive || fireflies[i].active;
  }

  int forceActivateIndex = anyFirefliesActive ? -1 : random(fireflyPinCount);

  for (int i = 0; i < fireflyPinCount; i++)
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
      }
    }

    int s = sin((float)firefly.progress / firefly.lifetime * PI) * 255;
    analogWrite(fireflyPins[i], firefly.active ? s : 0);
  }

  auto glowScale = (sin((millis() % 4000) / 4000.f * 2 * PI) * 3 / 8 + 0.625);
  auto color = Color3F(255, 255, 64) * glowScale;
  pixels.setPixelColor(0, color.R, color.G, color.B);
  pixels.show();

  delay(10);
}

