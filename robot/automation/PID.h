/*
PID class for certain automated tasks.

PID object is created with desired PID gains. Object is continuously updated
with a target value and a measured value to compute error over time.

Classes that use this are:
LineFollow
WallFollow
DriveTrajectory
RackTrajectory

*/

#ifndef PID_H
#define PID_H

class PID {
  public:
    PID(float kp, float ki, float kd)
    : _kp(kp), _ki(ki), _kd(kd),
      _P(0), _I(0), _D(0),
      _prevError(0), _error(0) {}

    // Call continuously
    float update(float targetVal, float measVal) {
      _t = micros();

      // Establish starting time on first call
      if (_tOld == 0){
        _tOld = _t;
        _prevError = targetVal - measVal;
        return 0;
      }

      _dt = (_t - _tOld) / 1000000.0f; // seconds
      if (_dt <= 0) {return 0;}  // for invalid dt measurements

      _error = targetVal - measVal;

      // P term
      _P = _kp*_error;

      // I term
      _I = 0;

      // D term
      _derivative = (_error - _prevError) / _dt;
      _D = _kd * _derivative;

      _prevError = _error;
      _tOld = _t;

      return _P + _I + _D;
    }

    void setGains(float kp, float ki, float kd){
      _kp = kp;
      _ki = ki;
      _kd = kd;
    }

    void reset() {
      _P = 0.0f;
      _I = 0.0f;
      _D = 0.0f;

      _error = 0.0f;
      _prevError = 0.0f;

      _integral = 0.0f;

      _dt = 0.0f;
      _tOld = 0;
    }

  private:
    float _kp, _ki, _kd; // PID gains
    float _P, _I, _D;    // PID terms
    float _prevError, _error;        // error terms
    float _integral;
    float _derivative;

    // time variables
    unsigned long _t, _tOld = 0; 
    float _dt = 0;  

};

#endif