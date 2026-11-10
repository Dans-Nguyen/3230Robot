/*
Trajectory class for the rack motor.

Initialize with physical motor parameters, motor encoder objects,
and PID objects.

Typical usage:
See robot.ino for use cases

*/

#ifndef RACK_TRAJECTORY_H
#define RACK_TRAJECTORY_H

class RackTrajectory {
  public:
  RackTrajectory(double GR, int countsPerRev, double rw,
                 Motor20D& motor, Encoder& enc, LimSwitch& limSwitch, PID& PID)
  : _GR(GR), _countsPerRev(countsPerRev), _rw(rw),
    _motor(motor), _enc(enc), _limSwitch(limSwitch), _PID(PID) {}

  // drive state variable
  enum STATE {ACTIVE, IDLE, HOMING};
  STATE state = IDLE;

  // Home rack
  void startHome() {
    state = HOMING;
  }

  // Set rack absolute position from home position
  // xf [cm], speed [cm/s]
  void setPosition(double xf, double speed) {
  double targetPosition = constrain(xf, 0, 20);  // cm from home

  if (speed <= 0.0) {
    return;  // prevent division by zero
  }

  // Read current rack position.
  _counts1 = _enc.read();
  _theta1 = 2.0 * PI * _counts1 / (_countsPerRev * _GR);

  _theta1_old = _theta1;
  _theta1_des = _theta1;

  // Absolute target, measured from home.
  _theta1_final = targetPosition / _rw;

  // Remaining linear distance, including direction.
  double currentPosition = _theta1 * _rw;
  double distanceToGo = targetPosition - currentPosition;

  // Time required at the requested linear speed [cm/s].
  _maxT = fabs(distanceToGo) / speed;

  // Handle an already-reached position.
  if (_maxT < 0.001) {
    _motor.setSpeed(0);
    state = IDLE;
    return;
  }

  _omega1_des = (_theta1_final - _theta1_des) / _maxT;

  _tStart = micros();
  _tOld = _tStart;

  state = ACTIVE;
  }

  void update(){
    if (state == IDLE) {return;}

    if (state == HOMING) {
      if (_limSwitch.state()) {    // limit switch is pressed
        _motor.setSpeed(0);
        _enc.write(0);             // define this location as home
        _counts1 = 0;
        _theta1 = 0.0;
        _theta1_old = 0.0;
        state = IDLE;
      }
      else {
        _motor.setSpeed(-100);      // drive toward the home switch
      }

      return;  // do not execute PID trajectory code while homing
    }

    _t = micros();
    _dt = (_t - _tOld) / 1000000.0f; // seconds

    float elapsedTime = (_t - _tStart) / 1000000.0f;

    // read encoder
    _counts1 = _enc.read();

    // compute angle
    _theta1 = 2.0f*PI*_counts1/(_countsPerRev*_GR);

    // compute angular velocity
    _omega1 = (_theta1 - _theta1_old) / _dt;

    if (elapsedTime >= _maxT){
      // Hold desired angles 
      _theta1_des = _theta1_final;
    } else {
      // Ramp desired angles
      _theta1_des = _theta1_des + _omega1_des*_dt;
    }

    // set motor command
    _mc = (int)(_PID.update(_theta1_des, _theta1));

    // once target is reached, stop driving
    if (fabs(_theta1_final - _theta1) < _positionTolerance &&
        fabs(_omega1) < _velocityTolerance &&
        elapsedTime >= _maxT) {

      _motor.setSpeed(0);
      _mc = 0;

      state = IDLE;

      return;
    }

    _motor.setSpeed(_mc);

    _tOld = _t;
    _theta1_old = _theta1;

    // if (((_t - _print_time)/1000000.0f) > 0.1) {  // non-blocking way to delay printing
    //   // print any variables of interest inside this if statement
    //   Serial.print(_theta1);
    //   Serial.print('\t');
    //   Serial.print(_theta1_des);
    //   Serial.print('\t');
    //   Serial.print(_theta1_final);
    //   Serial.print('\t');
    //   Serial.print(constrain(_mc, -255, 255));
    //   Serial.print('\t');
    //   Serial.println(elapsedTime);


    //   _print_time = _t;
    // }
  }  

  void reset() {
    _enc.write(0);

    _PID.reset();

    _tOld = micros();
    _tStart = micros();
    _dt = 0.0f;

    _counts1 = 0;

    _theta1 = 0.0;

    _theta1_old = 0.0;

    _theta1_des = 0.0;
  }
  

  private:
  // Required objects
  Motor20D& _motor;
  DriveControl& _drive;
  Encoder& _enc;
  LimSwitch& _limSwitch;
  PID& _PID;

  // Physical Parameters
  double _GR;         // gear ratio
  int _countsPerRev;  // encoder counts per Rev
  double _rw;         // wheel radius in cm
  double _D;          // distance between wheels in cm

  double _theta1, _theta1_old = 0, _omega1;   //position and velocity of wheel 1
  double _theta1_des = 0;   // desired position of wheels
  double _theta1_final;     // final desired position of wheels
  double _omega1_des;          // desired velocity of wheels

  long _counts1;  // encoder counts

  const double _positionTolerance = 0.05;  // rad
  const double _velocityTolerance = 0.05;  // rad/s

  // Motor speed command
  int _mc = 0;

  // time variables
  unsigned long _t, _tOld, _print_time, _tStart = 0; 
  double _maxT;
  float _dt = 0;

};

#endif