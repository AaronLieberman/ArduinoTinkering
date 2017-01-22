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

  Color3F operator*(float s)
  {
    return Color3F(R * s, G * s, B * s);
  }

  Color3F operator/(float s)
  {
    return Color3F(R / s, G / s, B / s);
  }

  Color3F operator+(Color3F o)
  {
    return Color3F(R + o.R, G + o.G, B + o.B);
  }
};

class Color3Fs
{
public:

  static Color3F White() { return Color3F(1, 1, 1); }
  static Color3F Black() { return Color3F(0, 0, 0); }
  static Color3F Orange() { return Color3F(1, 0.5f, 0); }
  static Color3F Yellow() { return Color3F(1, 1, 0); }
  static Color3F Purple() { return Color3F(1, 0, 1); }
  static Color3F Red() { return Color3F(1, 0, 0); }
  static Color3F Blue() { return Color3F(0, 0, 1); }
  static Color3F Green() { return Color3F(0, 1, 0); }

};

