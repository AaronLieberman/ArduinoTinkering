#pragma once
#include <Arduino.h>

class Color3F
{
public:

  float R;
  float G;
  float B;

  Color3F() : R(0), G(0), B(0)
  {
  }

  Color3F(float r, float g, float b)
  {
    R = r;
    G = g;
    B = b;
  }
};

