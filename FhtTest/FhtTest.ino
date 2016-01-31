/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

#define LOG_OUT 1 // use the log output function
#define FHT_N 256

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <FHT.h> // include the library

#include "Color3F.h"

#define PIN            6
#define NUMPIXELS      7
Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte _pixelBins[NUMPIXELS];

void setup()
{
  const int analogReferenceMode = EXTERNAL;
  const int audioPin = 0;

  Serial.begin(9600); // use the serial port

  _pixels.begin(); // This initializes the NeoPixel library.
  
  //TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = (analogReferenceMode << 6) | (audioPin & 0x07);
  DIDR0 = 0x01; // turn off the digital input for adc0

  memset(_pixelBins, 0, sizeof(_pixelBins));
}

void PrintData(byte *data, int count)
{
  static int lastPrint = 0;
  
  if (millis() > lastPrint + 100)
  {
    char buf[256];
    char *loc = buf;
    for (int i = 0; i < NUMPIXELS; i++)
    {
      sprintf(loc, "%d ", _pixelBins[i]);
      loc += strlen(loc);
    }
    
    lastPrint = millis();

    Serial.println(buf);
  }
}

void ShowValues(byte *data, int count)
{
  byte _newBinValues[NUMPIXELS];
  memset(_newBinValues, 0, NUMPIXELS);

  for (int i = 0; i < count; i++)
  {
    int binIndex = (int)((float)i / count * NUMPIXELS);
    _newBinValues[binIndex] += (float)data[i] * NUMPIXELS / count;
  }

  const float degrade = 0.7f;
  
  for (int i = 0; i < NUMPIXELS; i++)
  {
    _pixelBins[i] = _pixelBins[i] * degrade + _newBinValues[i] * (1.0f - degrade);
  }
  
  static int lastPrint = 0;
  
  if (millis() > lastPrint + 100)
  {    
    for (int i = 0; i < NUMPIXELS; i++)
    {
      Color3F c = Color3F(255, 255, 255);
      float brightness = (float)_pixelBins[i] / 255;
      _pixels.setPixelColor(i, _pixels.Color(c.R * brightness, c.G * brightness, c.B * brightness));
    }
  
    _pixels.show();
  }
}

void loop()
{
  while (true)
  {
    cli();  // UDRE interrupt slows this way down on arduino1.0
    
    for (int i = 0 ; i < FHT_N ; i++)
    {
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
    }
  
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    sei();

    PrintData(fht_log_out, FHT_N / 2);

    ShowValues(fht_log_out, FHT_N / 2);
  }
}

