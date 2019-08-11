#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

#include "stlhelper.h"

#include "Color3F.h"
#include "LatchButton.h"
#include "serialPrintf.h"
#include "AveragingFilter.h"

const int kNumNeoPixels = 10;

LatchButton _buttonLeft(4);
LatchButton _buttonRight(5);

AveragingFilter _filterX(10);
AveragingFilter _filterY(10);
AveragingFilter _filterZ(10);

bool _isActive = true;

void setup()
{
  CircuitPlayground.begin();
  Serial.begin(115200);

	_buttonLeft.initialize();
  _buttonRight.initialize();

  serialPrintfln("Setup complete");
}

void loop()
{
  if (_buttonLeft.getAndClearState())
  {
    _isActive = !_isActive;
  }

  if (_isActive)
  {
    float brightness = CircuitPlayground.slideSwitch() ? 1.f : 0.25f;
  
    auto x = CircuitPlayground.motionX();
    auto y = CircuitPlayground.motionY();
    auto z = CircuitPlayground.motionZ();
    auto fx = _filterX.addSample(x);
    auto fy = _filterY.addSample(y);
    auto fz = _filterZ.addSample(z);
  
    //serialPrintfln("%f, %f, %f", x, y, z);
    //serialPrintfln("%f, %f, %f", fx, fy, fz);
  
    bool active = fz < 0 || (abs(fz) < 3 && sqrt(fx * fx + fy * fy) > 6);
  
    uint32_t color = 0;
  
    if (!active)
    {
      float glowScale = sin((float)millis() / 1000 * PI) * 0.3f + 0.7f;
      brightness *= glowScale;
  
      color = 0x4000ff;
    }
    else
    {
      float r = 0;
      float g = sin((float)millis() / 100 * 2 * PI) * 0.5f + 0.5f;
      float b = 0;
      
      color = (uint8_t)(r * 255) << 16 | (uint8_t)(g * 255) << 8 | (uint8_t)(b * 255);
    }
  
    CircuitPlayground.setBrightness(brightness * 255);
  
  	for (int i = 0; i < kNumNeoPixels; i++)
  	{
      CircuitPlayground.setPixelColor(i, color);
  	}
  }
  else
  {
    CircuitPlayground.clearPixels();
  }

  delay(10);
}
