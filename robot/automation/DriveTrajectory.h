/*
Trajectory class for the drivetrain motors.

Initialize with physical motor parameters, motor encoder objects,
and PID objects.

Typical usage:
See robot.ino for use cases

*/

#ifndef DRIVE_TRAJECTORY_H
#define DRIVE_TRAJECTORY_H

class DriveTrajectory {
  public:
  DriveTrajectory(double GR, int countsPerRev, double rw, double D,
             DriveControl& driveControl, Encoder& enc1, Encoder& enc2, 
             PID& PID1, PID& PID2)
  : _GR(GR), _countsPerRev(countsPerRev), _rw(rw), _D(D),
    _drive(driveControl), _enc1(enc1), _enc2(enc2), 
    _PID1(PID1), _PID2(PID2) {}

  // drive state variable
  enum STATE {ACTIVE, IDLE};
  STATE state = IDLE;

  // Drive straight some distance in cm
  void startStraight(double xf, double maxT) {
    state = ACTIVE;

    _maxT = maxT;

    _theta1_final = xf/_rw; 
    _theta2_final = xf/_rw;

    _omega1_des = _theta1_final/maxT; // rad/s
    _omega2_des = _theta2_final/maxT; // rad/s 
  }

  // rotate in place some radians phi
  void startRotate(double phi, double maxT) {
    state = ACTIVE;

    _maxT = maxT;

    _theta1_final = -phi*_D*0.5/_rw; 
    _theta2_final = phi*_D*0.5/_rw;

    _omega1_des = _theta1_final/maxT; // rad/s
    _omega2_des = _theta2_final/maxT; // rad/s 
  }

  // Drive in circle of radius r for phi radians 
  void startCurve(double phi, double r, double maxT) {
    state = ACTIVE;

    _maxT = maxT;
    
    _theta1_final = phi*r/_rw; 
    _theta2_final = phi*(r+_D)/_rw;

    _omega1_des = _theta1_final/maxT; // rad/s
    _omega2_des = _theta2_final/maxT; // rad/s 
  }

  void update(){
    if (state == IDLE) {return;}
    

    _t = micros();
    _dt = (_t - _tOld) / 1000000.0f; // seconds

    float elapsedTime = (_t - _tStart) / 1000000.0f;

    // read encoders
    _counts1 = _enc1.read();
    _counts2 = _enc2.read();

    // compute angle
    _theta1 = 2.0f*PI*_counts1/(_countsPerRev*_GR);
    _theta2 = 2.0f*PI*_counts2/(_countsPerRev*_GR);

    // compute angular velocity
    _omega1 = (_theta1 - _theta1_old) / _dt;
    _omega2 = (_theta2 - _theta2_old) / _dt;

    if (elapsedTime >= _maxT){
      // Hold desired angles 
      _theta1_des = _theta1_final;
      _theta2_des = _theta2_final;
    } else {
      // Ramp desired angles
      _theta1_des = _theta1_des + _omega1_des*_dt;
      _theta2_des = _theta2_des + _omega2_des*_dt;
    }
    
    // set motor commands
    _m1c = (int)(_PID1.update(_theta1_des, _theta1));
    _m2c = (int)(_PID2.update(_theta2_des, _theta2)); 

    // Once position error and velocity is small, stop trajectory
    if (fabs(_theta1_final - _theta1) < _positionTolerance &&
        fabs(_theta2_final - _theta2) < _positionTolerance &&
        fabs(_omega1) < _velocityTolerance &&
        fabs(_omega2) < _velocityTolerance &&
        elapsedTime >= _maxT) {

      _drive.stop();
      _m1c = 0;
      _m2c = 0;

      state = IDLE;

      return;
    }

    _drive.setSpeeds(_m1c, _m2c);

    _tOld = _t;
    _theta1_old = _theta1;
    _theta2_old = _theta2;


    // if (((_t - _print_time)/1000000.0f) > 0.1) {  // non-blocking way to delay printing
    //   // print any variables of interest inside this if statement
      // Serial.print(_theta1);
      // Serial.print('\t');
      // Serial.print(_theta1_des);
      // Serial.print('\t');
      // Serial.print(_theta1_final);
      // Serial.print('\t');
      // Serial.print(constrain(_m1c, -400, 400));
      // Serial.print('\t');
      // Serial.print(_theta2);
      // Serial.print('\t');
      // Serial.print(_theta2_des);
      // Serial.print('\t');
      // Serial.print(_theta2_final);
      // Serial.print('\t');
      // Serial.print(constrain(_m2c, -400, 400));
      // Serial.print('\t');
      // Serial.println(elapsedTime);


    //   _print_time = _t;
    // }
  }  

  void reset() {
    _enc1.write(0);
    _enc2.write(0);

    _PID1.reset();
    _PID2.reset();

    _tOld = micros();
    _tStart = micros();
    _dt = 0.0f;

    _counts1 = 0;
    _counts2 = 0;

    _theta1 = 0.0;
    _theta2 = 0.0;

    _theta1_old = 0.0;
    _theta2_old = 0.0;

    _theta1_des = 0.0;
    _theta2_des = 0.0;
  }
  

  private:
  // Required objects
  DriveControl& _drive;
  Encoder& _enc1;
  Encoder& _enc2;
  PID& _PID1;
  PID& _PID2;

  // Physical Parameters
  double _GR;         // gear ratio
  int _countsPerRev;  // encoder counts per Rev
  double _rw;         // wheel radius in cm
  double _D;          // distance between wheels in cm

  double _theta1, _theta1_old = 0, _omega1;   //position and velocity of wheel 1
  double _theta2, _theta2_old = 0, _omega2;   //position and velocity of wheel 2
  double _theta1_des = 0, _theta2_des = 0;    // desired position of wheels
  double _theta1_final, _theta2_final;      // final desired position of wheels
  double _omega1_des, _omega2_des;            // desired velocity of wheels

  long _counts1, _counts2;  // encoder counts

  const double _positionTolerance = 0.05;  // rad
  const double _velocityTolerance = 0.05;  // rad/s

  // Motor speed command
  int _m1c = 0;
  int _m2c = 0;

  // time variables
  unsigned long _t, _tOld, _print_time, _tStart = 0; 
  double _maxT = 0;
  float _dt = 0;

};

#endif