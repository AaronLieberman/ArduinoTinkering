#pragma once

class MicSampler
{
private:

  const int _sampleWindow = 50;

  int _dataPin;
  unsigned long _sampleStartMillis = 0;
  unsigned int _signalMax = 0;
  unsigned int _signalMin = 0;
  float _currentSignal;

public:

  MicSampler(int dataPin);
  void Initialize();
  float GetSample();
  
};

