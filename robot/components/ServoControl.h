#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Servo.h>

class ServoControl {
  private:
    Servo servo;
    int pin;
    int angle;

  public:
    ServoControl(int servoPin) : pin(servoPin), angle(90) {}

    void begin() {
      servo.attach(pin);
      servo.write(angle);
    }

    void setAngle(int newAngle) {
      angle = constrain(newAngle, 0, 180);
      servo.write(angle);
    }

    int getAngle() {
      return angle;
    }
};

#endif


