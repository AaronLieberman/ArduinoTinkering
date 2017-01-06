#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "Color3F.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

#define STRANDLENGTH 7
#define NUMSTRANDS 4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS STRANDLENGTH * NUMSTRANDS

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

float hue = 0;
const float hueOffset = 0.01f;
float phase = 0;
const float phaseOffset = 1.0f / 167;

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

void patternHue()
{
  for (int j = 0; j < 700; j++)
  {
    for(int i = 0; i < NUMPIXELS; i++)
    {
      Color3F c = GenerateHue(hue + ((float)i) / NUMPIXELS);
  
      float brightness = cos((phase + 1.0f * i / NUMPIXELS) * 2 * M_PI) / 2 + 0.5f;
      pixels.setPixelColor(i, pixels.Color(c.R * brightness, c.G * brightness, c.B * brightness));
  
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  
    delay(1); // Delay for a period of time (in milliseconds).
  
    hue = hue + hueOffset >= 1 ? 0 : hue + hueOffset;
    phase = phase + phaseOffset >= 1 ? 0 : phase + phaseOffset;
  }
}

void patternBlink()
{
  for (int i = 0; i < 7  ; i++)
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
}

void patternFlicker()
{
  for (int i = 0; i < 7; i++)
  {
    setPixels(yellow, 0, NUMPIXELS);
    delay(2000 + random(10) * 1000);

    setPixels(black, 0, NUMPIXELS);
    delay(50);
    setPixels(yellow, 0, NUMPIXELS);
    delay(100);
    setPixels(black, 0, NUMPIXELS);
    delay(50);
  }
}

void patternParade()
{
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < NUMSTRANDS; j++)
    {
      setPixels(black, 0, NUMPIXELS);
      setPixels(white, j * STRANDLENGTH, (j + 1) * STRANDLENGTH);
      delay(200);
    }
  }
}

void patternParadeAndBack()
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 1; j < NUMSTRANDS; j++)
    {
      setPixels(black, 0, NUMPIXELS);
      setPixels(white, j * STRANDLENGTH, (j + 1) * STRANDLENGTH);
      delay(200);
    }

    for (int j = NUMSTRANDS - 2; j >= 0; j--)
    {
      setPixels(black, 0, NUMPIXELS);
      setPixels(white, j * STRANDLENGTH, (j + 1) * STRANDLENGTH);
      delay(200);
    }
  }
}

void loop()
{
  switch (random(0, 5))
  {
    case 0: patternBlink(); break;
    case 1: patternFlicker(); break;
    case 2: patternParade(); break;
    case 3: patternParadeAndBack(); break;
    case 4: patternHue(); break;
  }
}

