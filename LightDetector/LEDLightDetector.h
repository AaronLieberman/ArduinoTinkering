class LEDLightDetector
{
public:
  LEDLightDetector(int pinPos, int pinNeg, unsigned long thresholdMicros = 100000) :
    m_pinPos(pinPos),
    m_pinNeg(pinNeg),
    m_thresholdMicros(thresholdMicros),
    m_isActive(false)
  {
    pinMode(pinPos, OUTPUT);
    pinMode(pinNeg, INPUT);

    m_endMicros = micros() + m_thresholdMicros;
  }

  void loop()
  {
    unsigned long now = micros();

    if (now >= m_endMicros)
    {
      m_isActive = digitalRead(m_pinNeg) == 0;
      
      // Apply reverse voltage, charge up the pin and led capacitance
      pinMode(m_pinNeg, OUTPUT);
      digitalWrite(m_pinNeg, HIGH);
      digitalWrite(m_pinPos, LOW);
      
      // Isolate the pin 2 end of the diode by changing it from OUTPUT HIGH to 
      // INPUT LOW (high impedance input with internal pull-up resistor off)
      pinMode(m_pinNeg, INPUT);
      digitalWrite(m_pinNeg, LOW);  // turn off internal pull-up resistor
      
      m_endMicros = now + m_thresholdMicros;
    }
  }

  bool isActive() const { return m_isActive; }

private:

  int m_pinPos;
  int m_pinNeg;
  unsigned long m_thresholdMicros;
  unsigned long m_endMicros = 0;
  bool m_isActive;
  
};
