/*
Component class for the limit switch. Only used for rack trajectory
Mega Pin: 53
*/

#ifndef LIM_SWITCH_H
#define LIM_SWITCH_H

class LimSwitch {
  public:
    LimSwitch(uint8_t pin) : _pin(pin) {}

    void begin() {
      pinMode(_pin, INPUT_PULLUP);
    }

    bool state() {
      return digitalRead(_pin);
    }

  private:
    uint8_t _pin;

};

#endif