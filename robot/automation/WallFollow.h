#ifndef WALL_FOLLOW_H
#define WALL_FOLLOW_H

class WallFollow{
  public:
    WallFollow(SharpGP2Y0A21YK0F& sensor, DriveControl& driveControl, PID& PID)
      : _distSensor(sensor), _drive(driveControl), _PID(PID) {}

    void start(int baseSpeed, float targetDist) {
      /*
      Starts wall following
      Inputs:
      - baseSpeed: base speed for motor command (-400 to 400)
      - targetDist: target distance from wall in cm
      */

      // get value from distance sensor
      float distCm = _distSensor.getDistCm();

      int m1c = (int)(baseSpeed + _PID.update(targetDist, distCm));
      int m2c = (int)(baseSpeed); //- _PID.update(targetDist, distCm));

      _drive.setSpeeds(m1c, m2c);
    }

  void stop() {
    _drive.stop();
  }


  private:
  SharpGP2Y0A21YK0F& _distSensor;
  DriveControl& _drive;
  PID& _PID;

};


#endif