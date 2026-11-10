#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

class LineFollow {
public:

  LineFollow(QTRMD08RC& sensor, DriveControl& driveControl, PID& PID)
    : _lineSensor(sensor), _drive(driveControl), _PID(PID) {}

  void start(int baseSpeed) {
    // update and get values from reflectance sensor
    _lineSensor.update();
    _lineSensor.getValues(_sensorVal);

    float num = 0.0f;
    float den = 0.0f;

    for (uint8_t i = 0; i < QTRMD08RC::SensorCount; i++) {
      num += _sensorVal[i] * _d[i];
      den += _sensorVal[i];
    }

    // No reliable line detection: stop rather than divide by zero.
    if (den <= 0.0f) {
      _drive.stop();
      return;
    }
    float linePos = num / den;

    //float error = linePosition - _targetPos;  // middle of the 8-sensor array


    int m1c = (int)(baseSpeed - _PID.update(_targetPos, linePos));
    int m2c = (int)(baseSpeed + _PID.update(_targetPos, linePos));

    _drive.setSpeeds(m1c, m2c);
  }

  void stop() {
    _drive.stop();
  }

private:
  QTRMD08RC& _lineSensor;
  DriveControl& _drive;
  PID& _PID;
  float _targetPos = 2.8;

  uint16_t _sensorVal[QTRMD08RC::SensorCount] = {};

  const float _d[QTRMD08RC::SensorCount] =
    {0.0f, 0.8f, 1.6f, 2.4f, 3.2f, 4.0f, 4.8f, 5.6f};
};

#endif
