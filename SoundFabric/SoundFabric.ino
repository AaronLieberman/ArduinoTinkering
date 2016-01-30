#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <stdlib.h>
#include <math.h>

#include "Color3F.h"
#include "MicSampler.h"

const int _pixelPin = 6;
const int _micPin = 0;
const int _numPixels = 7;

long _lastPrint = 0;

Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(_numPixels, _pixelPin, NEO_GRB + NEO_KHZ800);
MicSampler _micSampler(_micPin);

void setup()
{
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.). Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V. COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);
  Serial.begin(9600);
  
  _micSampler.Initialize();

  _pixels.begin(); // This initializes the NeoPixel library.
}

void loop()
{
  float sample = _micSampler.GetSample();

  char tmpSample[20];
  dtostrf(sample, 6, 4, tmpSample);

  float scale = (float)analogRead(1);
  char tmpScale[20];
  dtostrf(scale, 6, 4, tmpScale);
  
  sample = max(log(sample) / log(scale) + 1, 0);
  char tmpLogSample[20];
  dtostrf(sample, 6, 4, tmpLogSample);

  if (millis() > _lastPrint + 1000)
  {
    char buf[128];
    sprintf(buf, "sample=%s scale=%s logSample=%s", tmpSample, tmpScale, tmpLogSample);
    Serial.println(buf);
    _lastPrint = millis();
  }

  for(int i = 0; i < _numPixels; i++)
  {
    Color3F c = Color3F(255, 255, 255);

    float brightness = min(max(sample - ((float)i / _numPixels), 0) * _numPixels, 1);
    _pixels.setPixelColor(i, _pixels.Color(c.R * brightness, c.G * brightness, c.B * brightness));
  }

  _pixels.show();
}

