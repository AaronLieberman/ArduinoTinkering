#pragma once

class LatchButton
{
private:

  int _buttonPin;
  bool _buttonLatched = false;

public:

  LatchButton(int buttonPin);

  void initialize();
  bool getAndClearState();
  
};
