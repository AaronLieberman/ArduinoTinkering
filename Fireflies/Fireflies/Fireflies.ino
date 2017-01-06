

const int pinPot = A0;
const int pins[] = { 3, 5, 6 };
const int pinCount = 3;


void setup()
{
  for (int i = 0; i < pinCount; i++)
  {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  int potValue = analogRead(pinPot);

  for (int i = 0; i < pinCount; i++)
  {
    int r = random(255 - 32) + 32;
    analogWrite(pins[i], r);
  }

  delay(potValue);
}

