/* 


Mega Pin : A2 
Uno  Pin : A2

*/

#ifndef HALL_EFFECT_H
#define HALL_EFFECT_H

class Hall_Effect {
public:
  Hall_Effect(uint8_t sensorPin) : _pin(sensorPin) {}

  // get initial reading
  void begin() {
    _rawVal = analogRead(_pin);
  }

  float getVal() {
    // quiescent value is 2.5V
    
    _rawVal = analogRead(_pin);

    // Convert to voltage
    _Volt = _rawVal * (5.0f / 1023.0f);

    return _Volt;
  }

private:
  uint8_t _pin;
  float _rawVal;
  float _Volt;
};

#endif