#pragma once

class PeriodicDebug
{
public:

  PeriodicDebug(int blinkInterval, int logInterval);

  void initialize();
  void update();

private:

  int _logInterval;
  int _blinkInterval;
  int _lastLogMillis = 0;
  int _lastBlinkMillis = 0;
  bool _lastBlinkState = false;
  
};
