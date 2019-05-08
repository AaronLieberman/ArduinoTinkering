#include <Arduino.h>
#include <Stepper.h>
#include <Adafruit_NeoPixel.h>

const int kStepperA1 = 7;
const int kStepperA2 = 9;
const int kStepperB1 = 11;
const int kStepperB2 = 10;
const int kNeoPixelsPin = 4;
const int kNumberOfPixels = 10;

Stepper motor(512, kStepperA1, kStepperA2, kStepperB1, kStepperB2);  
Adafruit_NeoPixel pixels(kNumberOfPixels, kNeoPixelsPin, NEO_GRB + NEO_KHZ800);

uint32_t _currentColors[kNumberOfPixels] = {};
uint32_t _targetColors[kNumberOfPixels] = {};

int _motorLoc = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(kStepperA1, OUTPUT);     
  pinMode(kStepperA2, OUTPUT);     
  pinMode(kStepperB1, OUTPUT);     
  pinMode(kStepperB2, OUTPUT);     

  pixels.begin();
  pixels.clear();
  motor.setSpeed(20);

  generateColors(_currentColors, kNumberOfPixels);
  generateColors(_targetColors, kNumberOfPixels);
}

void generateColors(uint32_t colors[], int count)
{
  for (int i = 0; i < count; i++)
  {
    colors[i] = pixels.Color(random(0, 16) * random(0, 16), random(0, 16) * random(0, 16), random(0, 16) * random(0, 16));
  }  
}

void rot(int deg)
{
  motor.step((float)deg / 360 * 200);
}

void rotTo(int deg)
{
  rot(deg - _motorLoc);
  _motorLoc = deg;
}

uint32_t blendComponent(uint32_t color1, uint32_t color2, int shift, float f)
{
  uint8_t c1 = (uint8_t)(color1 >> shift);
  uint8_t c2 = (uint8_t)(color2 >> shift);
  uint8_t r = (uint8_t)((float)c1 * f + (float)c2 * (1.0f - f));
  return r << shift;
}

void applyColors()
{
  float f = 0.95f;
  for (int i = 0; i < kNumberOfPixels; i++)
  {
    _currentColors[i] =
      blendComponent(_currentColors[i], _targetColors[i], 16, f) |
      blendComponent(_currentColors[i], _targetColors[i], 8, f) |
      blendComponent(_currentColors[i], _targetColors[i], 0, f);

    pixels.setPixelColor(i, _currentColors[i]);
  }

  pixels.show();
  Serial.println((uint8_t)(_currentColors[0] >> 16));
}

void play1()
{
  rotTo(random(-75, 75));
}

void play2()
{
  for (int i = 0; i < 100; i++)
  {
    applyColors();
    delay(3);
  }

  generateColors(_targetColors, kNumberOfPixels);
}

void loop()
{
  play1();
  play2();
  
  //delay(random(400, 2000));
}

