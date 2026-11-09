/* IR Phototransistor class for LTR_4206E sensor

Pin 25
*/

#ifndef LTR_4206E_IR_H
#define LTR_4206E_IR_H


class LTR_4206E_IR {
  public:
  // Pin setup (constructor)
  LTR_4206E_IR(uint8_t sensorPin, unsigned long timeout = 20000UL)
      : _pin(sensorPin), _timeout(timeout),
        _highTime(0), _lowTime(0),
        _frequency(0.0f) {}

  // initialize sensor
  void begin() {
    pinMode(_pin, INPUT_PULLUP);
  }
  // create functions

  //set pulse In timeout (in microseconds)
  void setTimeout(unsigned long timeout){
    _timeout = timeout;
  }

  //read the sensor
  float getFrequency() {

    _pulseTime = pulseIn(_pin, LOW, _timeout) + pulseIn(_pin, HIGH, _timeout);

    if (_pulseTime > 0) {
      _frequency = 1 / (float(_pulseTime)) * 1000000.0;
    } else {
      _frequency = 0;
    }

    return _frequency;
  }

private:
  uint8_t _pin;
  unsigned long _timeout;

  unsigned long _highTime;
  unsigned long _lowTime;

  float _frequency;
  long _pulseTime;

};

#endif
