// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <stdlib.h>

#include "Color3F.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
const int _pixelPin = 6;
const int _micPin = 5;

// How many NeoPixels are attached to the Arduino?
const int _numPixels = 7;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(_numPixels, _pixelPin, NEO_GRB + NEO_KHZ800);

const int _sampleWindow = 50;
unsigned long _sampleStartMillis = 0;
unsigned int _signalMax = 0;
unsigned int _signalMin = 0;
float _currentSignal;

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

  _pixels.begin(); // This initializes the NeoPixel library.
}

float _hue = 0;
const float _hueOffset = 0.01f;
float _phase = 0;
const float _phaseOffset = 1.0f / 167;

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

float GetSample()
{
  long now = millis();

  if (now - _sampleStartMillis > _sampleWindow)
  {
    unsigned int peakToPeak = _signalMax - _signalMin; // max - min = peak-peak amplitude
    _currentSignal = (double)peakToPeak / 1024;

    char buf[128];
    char tempSignalString[20];
    dtostrf(_currentSignal, 6, 4, tempSignalString);
    sprintf(buf, "%s %d %d", tempSignalString, _signalMin, _signalMax);
    Serial.println(buf);

    _sampleStartMillis = now;
    _signalMin = 1024;
    _signalMax = 0;
  }
  else
  {
     unsigned int sample = analogRead(0);
     
     if (sample < 1024)  // toss out spurious readings
     {
        if (sample > _signalMax)
        {
           _signalMax = sample;  // save just the max levels
        }
        else if (sample < _signalMin)
        {
           _signalMin = sample;  // save just the min levels
        }
     }
  }

  return _currentSignal;
}

void loop()
{
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  float sample = GetSample();
  //sample = (float)analogRead(1) / 1023;
  //Serial.println(sample);
  float sampleDistance = 1.f / _numPixels;

  for(int i = 0; i < _numPixels; i++)
  {
    //Color3F c = GenerateHue(_hue + ((float)i) / _numPixels);
    Color3F c = Color3F(255, 255, 255);

//TODO Log scale
//TODO gain

    float brightness = min(max(sample - ((float)i / _numPixels), 0) * _numPixels, 1);
    _pixels.setPixelColor(i, _pixels.Color(c.R * brightness, c.G * brightness, c.B * brightness));

    _pixels.show();

    delay(1);
  }

  _hue = _hue + _hueOffset >= 1 ? 0 : _hue + _hueOffset;
  _phase = _phase + _phaseOffset >= 1 ? 0 : _phase + _phaseOffset;
}

