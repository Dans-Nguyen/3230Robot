/*
Servo class for the micro and MG servo (forgot their names, look them up)

Sets and gets the angles for a servo

Typical usagec:\Users\dansn\Documents\GitHub\3230Robot\robot\components\SharpGP2Y0A21YK0F.h:c:\Users\dansn\Documents\GitHub\3230Robot\robot\components\DistanceSensor.h

ServoControl armServo(11, 90);     // Pin 11, start at 90°
ServoControl leverServo(13, 45);   // Pin 13, start at 45°

// In setup
armServo.begin();
leverServo.begin();

// In loop
armServo.setAngle(120);            // Sets angle to 120°
angle = leverServo.getAngle();     // Gets current angle

*/

#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Servo.h>

class ServoControl {
  public:
    // Set servo pin and initial angle in the constructor
    ServoControl(int servoPin, int initAngle) : _pin(servoPin), _angle(constrain(initAngle, 0, 180)) {}

    // initialize servo
    void begin() {
      servo.attach(_pin);
      servo.write(_angle);
    }

    void setAngle(int newAngle) {
      _angle = constrain(newAngle, 0, 180);
      servo.write(_angle);
    }

    int getAngle() const {
      return _angle;
    }

  private:
    Servo servo;
    int _pin;
    int _angle;
};

#endif


