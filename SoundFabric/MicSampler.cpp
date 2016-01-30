#pragma once
#include <Arduino.h>
#include "MicSampler.h"

MicSampler::MicSampler(int dataPin) :
  _dataPin(dataPin)
{
}

void MicSampler::Initialize()
{
  pinMode(_dataPin, INPUT);
}

float MicSampler::GetSample()
{
  long now = millis();

  if (now - _sampleStartMillis > _sampleWindow)
  {
    unsigned int peakToPeak = _signalMax - _signalMin; // max - min = peak-peak amplitude
    _currentSignal = (float)peakToPeak / 1024;

    char tempSignalString[20];
    dtostrf(_currentSignal, 6, 4, tempSignalString);

    float logSample = log10(_currentSignal);
    char tempLogSignalString[20];
    dtostrf(logSample, 6, 4, tempLogSignalString);

    char buf[128];
    sprintf(buf, "min=%d max=%d signal=%s logsignal=%s", _signalMin, _signalMax, tempSignalString, tempLogSignalString);
    //Serial.println(buf);

    _sampleStartMillis = now;
    _signalMin = 1024;
    _signalMax = 0;
  }
  else
  {
     unsigned int sample = analogRead(_dataPin);
     
     if (sample < 1024)
     {
        if (sample > _signalMax)
        {
           _signalMax = sample;
        }
        else if (sample < _signalMin)
        {
           _signalMin = sample;
        }
     }
  }

  return _currentSignal;
}

