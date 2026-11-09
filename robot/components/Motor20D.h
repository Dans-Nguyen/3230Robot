/*
Motor class for Pololu 20D motor

Arduino mega pins:
IN1 = 5
IN2 = 6
*/

#ifndef MOTOR_20D_H
#define MOTOR_20D_H

class Motor20D{
  public:
    Motor20D(uint8_t IN1, uint8_t IN2) : _in1(IN1), _in2(IN2) {}

    // Set speed to 0
    void begin() {
      analogWrite(_in1, 0);
      analogWrite(_in2, 0);
    }

    // Set speed. Magnitude sets magnitude and sign sets direction
    void setSpeed(int speedValue){
      speedValue = constrain(speedValue, -255, 255);

      if (speedValue > 0) {
        analogWrite(_in1, speedValue);
        analogWrite(_in2, 0);
      }
      else if (speedValue < 0) {
        analogWrite(_in1, 0);
        analogWrite(_in2, -speedValue);
      }
      else {
        analogWrite(_in1, 0);
        analogWrite(_in2, 0);  // brake
      }
    }
    

  private:
    uint8_t _in1;
    uint8_t _in2;
};

#endif