void setup()
{
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x00; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

const int _sampleCount = 256;
byte _samples[_sampleCount * 2];

void loop()
{
  // reduces jitter
  while(true)
  {
    cli();  // UDRE interrupt slows this way down on arduino1.0

    for (int sampleIndex = 0; sampleIndex < _sampleCount; sampleIndex++)
    {
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      // restart adc
      ADCSRA = 0xf5 | bit (ADPS0) | bit (ADPS1) | bit (ADPS2);
      byte m = ADCL; // fetch adc data
      byte j = ADCH;

      _samples[sampleIndex * 2] = m;
      _samples[sampleIndex * 2 + 1] = j;
    }
    
    sei();
    
    Serial.write(255); // send a start byte
    Serial.write(254); // send a start byte
    Serial.write(255); // send a start byte
    Serial.write(254); // send a start byte

    Serial.write(_samples, _sampleCount * 2);
  }
}

