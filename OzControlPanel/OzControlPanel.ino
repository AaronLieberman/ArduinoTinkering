#include <Arduino.h>
#include <Scheduler.h>
#include <Stepper.h>
#include <Adafruit_NeoPixel.h>

const int kStepperA1 = 7;
const int kStepperA2 = 9;
const int kStepperB1 = 11;
const int kStepperB2 = 10;
const int kNeoPixelsPin = 4;
const int kNumberOfNeoPixels = 4;

Stepper motor(512, kStepperA1, kStepperA2, kStepperB1, kStepperB2);  
Adafruit_NeoPixel pixels(kNumberOfNeoPixels, kNeoPixelsPin, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(115200);

  pinMode(kStepperA1, OUTPUT);     
  pinMode(kStepperA2, OUTPUT);     
  pinMode(kStepperB1, OUTPUT);     
  pinMode(kStepperB2, OUTPUT);     

  pixels.begin();
  pixels.clear();
  motor.setSpeed(10);

  Scheduler.startLoop(loop2);
//  OCR0A = 0xAF;
//  TIMSK0 |= _BV(OCIE0A);
}

//bool _ledOn = false;
//int _millisTillNextUpdate = 0;
//SIGNAL(TIMER0_COMPA_vect)
//{
//  if (_millisTillNextUpdate <= 0)
//  {
//    _ledOn = !_ledOn;
//    digitalWrite(LED_BUILTIN, _ledOn);
//  }
//  
//  _millisTillNextUpdate--;
//}

void loop2()
{
  static bool ledOn = false;
  ledOn = !ledOn;
  digitalWrite(LED_BUILTIN, ledOn);
  delay(500);
}

void rot(int deg)
{
  motor.step((float)deg / 360 * 200);
}

void play1()
{
  rot(45);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.show();
  rot(-90);
  pixels.setPixelColor(1, pixels.Color(0, 150, 0));
  pixels.show();
  rot(90);
  pixels.setPixelColor(2, pixels.Color(0, 150, 0));
  pixels.show();
  rot(-90);
  pixels.setPixelColor(3, pixels.Color(0, 150, 0));
  pixels.show();
  rot(45);
  pixels.clear();
  pixels.show();
}

void loop()
{
  if (Serial.available())
  {
    int steps = Serial.parseInt();

    if (steps == 1)
    {
      play1();
    }
    else
    {
      motor.step(steps);
      Serial.println(steps);
    }
  }
}
