#define LIN_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

#include <FHT.h> // include the library

void setup()
{
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x00; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

int GetBin(int value)
{
  // Arduino pow function is approximate. Easier to just use a LUT
  if (value < 2) return 1;
  if (value < 4) return 2;
  if (value < 8) return 3;
  if (value < 16) return 4;
  if (value < 32) return 5;
  if (value < 64) return 6;
  if (value < 128) return 7;
  if (value < 256) return 8;
  return 9;
}

void loop()
{
  // reduces jitter
  while(true)
  {
    cli();  // UDRE interrupt slows this way down on arduino1.0

    const int multiplier = 1;
    int sampleIndex = 0;

    // save 256 samples
    for (int i = 0 ; i < FHT_N * multiplier ; i++)
    {
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      // restart adc
      ADCSRA = 0xf5;
      ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);
      //ADCSRA = 0xf5 | bit (ADPS0) | bit (ADPS1) | bit (ADPS2);
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int

      if (i % multiplier == 0)
      {
        fht_input[sampleIndex++] = k; // put real data into bins
      }
    }
    
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin(); // take the output of the fht
    sei();
    
    Serial.write(255); // send a start byte
    Serial.write(254); // send a start byte
    Serial.write(255); // send a start byte
    Serial.write(254); // send a start byte

    const int binCount = FHT_N/2;

    uint16_t logBins[binCount];
    //TODO Why isn't memset(logBins, 0, sizeof(logBins)) working?
    for (int i = 0; i < binCount; i++) { logBins[i] = 0; }

    const int noiseLevel = 12;

    // convert the data to log-linear
    for (int i = 0; i < binCount; i++)
    {
      int binIndex = GetBin(i);
      //logBins[binIndex] += fht_lin_out[i] - min(fht_lin_out[i], noiseLevel);
      logBins[i] = fht_lin_out[i] - min(fht_lin_out[i], noiseLevel);
    }

    // convert the data to log-log
    byte data[binCount];
    for (int i = 0; i < binCount; i++)
    {
      data[i] = 20 * log10(logBins[i]);
      //data[i] = logBins[i];
    }

    // send out the data in log-log
    Serial.write(data, binCount);
  }
}

