/*
QTR-MD-08RC_H
*/

#ifndef QTRMD08RC_H
#define QTRMD08RC_H

#include <QTRSensors.h>

class QTRMD08RC{
  public:
  static constexpr uint8_t SensorCount = 8;

  QTRMD08RC(const uint8_t (&sensorPins)[SensorCount]) {
    for (uint8_t i = 0; i < SensorCount; i++) {
      _pins[i] = sensorPins[i];
    }
  }

  // Initialize sensor
  void begin() {
    qtr.setTypeRC();
    qtr.setSensorPins(_pin, SensorCount);
  }

  float getVal() {
    // Get value from sensor
    qtr.read(_rawVal)

    // offset raw value
    for (int i = 0; i == SensorCount; i++){
      _unbiasedVal[i] = _rawVal[i] - sensor_bias[i];
    }

    return _unbiasedVal;
  }

  private:
    QTRSensors _qtr;
    uint8_t _pins[SensorCount];
    uint16_t _rawValues[SensorCount];
};

#endif