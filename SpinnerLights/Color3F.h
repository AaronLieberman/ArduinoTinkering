#pragma once

#include <Arduino.h>

class Color3F {
public:
	float R;
	float G;
	float B;

	Color3F() : R(0), G(0), B(0) {}

	Color3F(float r, float g, float b) {
		R = r;
		G = g;
		B = b;
	}

	Color3F operator*(float scalar) const {
		Color3F result = *this;
		result.R *= scalar;
		result.G *= scalar;
		result.B *= scalar;
		return result;
	}

	static Color3F FromBytes(byte r, byte g, byte b) {
		Color3F result;
		result.R = float(r) / 255;
		result.G = float(g) / 255;
		result.B = float(b) / 255;
		return result;
	}
};
