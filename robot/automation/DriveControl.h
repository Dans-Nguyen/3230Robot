// DriveControl.h
#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#include "DualTB9051FTGMotorShieldMod3230.h"

class DriveControl {
public:

  void begin() {
    md.init();
    md.enableDrivers();
    md.flipM1(true);
    md.setM1Speed(0);
    md.setM2Speed(0);
  }

  void forward(int speed) {
    md.setM1Speed(speed);
    md.setM2Speed(speed);
  }

  void backward(int speed) {
    md.setM1Speed(-speed);
    md.setM2Speed(-speed);
  }

  void turnCW(int speed) {
    md.setM1Speed(speed);
    md.setM2Speed(-speed);
  }

  void turnCCW(int speed) {
    md.setM1Speed(-speed);
    md.setM2Speed(speed);
  }

  void setSpeeds(int m1Speed, int m2Speed) {
    md.setM1Speed(m1Speed);
    md.setM2Speed(m2Speed);
  }

  void stop() {
    md.setM1Speed(0);
    md.setM2Speed(0);
  }

private:
  DualTB9051FTGMotorShieldMod3230 md;
};

#endif