/* IR Phototransistor class for LTR_4206E sensor

Pin A3
*/

#ifndef LTR_4206E_IR_H
#define LTR_4206E_IR_H
#include <Arduino.h>
// Might not need ^^^ just good practice

class LTR_4206E_IR {
  public:
  // Pin setup (constructor)
  LTR_4206E_IR(uint8_t sensorPin, unsigned long timeout = 20000UL)
      : _pin(sensorPin), _timeout(timeout),
        _highTime(0), _lowTime(0),
        _frequency(0.0f) {}

// initialize sensor
void begin() {
  pinMode(_pin, INPUT_PULLUP)
}
// create functions

//set pulse In timeout (in microseconds)
void setTimeout(unsigned long timeout){
  _timeout = timeout;
}

//read the sensor
foat getFrequency() {

  //measure the high and low. then check the the pulses were detected

  //Measuring the low pulse time
  _lowTime = pulseIn(_pin, LOW, _timeout);
  //Measuring the high pulse time
   _highTime = pulseIn(_pin, HIGH, _timeout);

  //check valid pulses 
  // if high pulse time is = 0 or low pulse time is = 0
   if (_highTime == 0 || _lowTime == 0) {
        _frequency = 0.0f;
        return _frequency;

}

// signal period calculations (in microseconds)
unsigned long period = _highTime + _lowTime;

// change period to frequency (in Hz)
_frequency = 1000000.0f / period;

return _frequency;
}


  private:
    uint8_t _pin;
    unsigned long _timeout;

    unsigned long _highTime;
    unsigned long _lowTime;

    float _frequency;

};

#endif

