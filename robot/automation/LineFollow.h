#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

class LineFollow {
public:

  LineFollow(QTRMD08RC& sensor, DriveControl& driveControl)
    : lineSensor(sensor), drive(driveControl) {}

  void start(int baseSpeed, float Kp) {
    // update and get values from reflectance sensor
    lineSensor.update();
    lineSensor.getValues(_sensorVal);

    float num = 0.0f;
    float den = 0.0f;

    for (uint8_t i = 0; i < QTRMD08RC::SensorCount; i++) {
      num += _sensorVal[i] * _d[i];
      den += _sensorVal[i];
    }

    // No reliable line detection: stop rather than divide by zero.
    if (den <= 0.0f) {
      drive.stop();
      return;
    }

    float linePosition = num / den;
    float error = linePosition - 2.8f;  // middle of the 8-sensor array

    int m1c = (int)(baseSpeed + Kp * error);
    int m2c = (int)(baseSpeed - Kp * error);

    drive.setSpeeds(m1c, m2c);
  }

  void stop() {
    drive.stop();
  }

private:
  QTRMD08RC& lineSensor;
  DriveControl& drive;

  uint16_t _sensorVal[QTRMD08RC::SensorCount] = {};

  const float _d[QTRMD08RC::SensorCount] =
    {0.0f, 0.8f, 1.6f, 2.4f, 3.2f, 4.0f, 4.8f, 5.6f};
};

#endif
