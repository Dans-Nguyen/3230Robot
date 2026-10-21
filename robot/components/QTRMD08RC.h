/*
Reflectance sensor class for the QTR-MD-08RC_H sensor

Reads analog voltage from the RC sensors and returns the value
of all sensors.

Typical usage:

QTRMD08RC lineSensor({37, 36, 39, 38, 41, 40, 43, 42});

uint16_t values[QTRMD08RC::SensorCount]; // Initilizes array of values
lineSensor.update();          // Update sensor
lineSensor.getValues(values); // Get values
*/

#ifndef QTRMD08RC_H
#define QTRMD08RC_H

#include <QTRSensors.h>

class QTRMD08RC{
  public:
  static constexpr uint8_t SensorCount = 8;
  static constexpr uint8_t sensorBias = 92;

  QTRMD08RC(const uint8_t (&sensorPins)[SensorCount]) {
    for (uint8_t i = 0; i < SensorCount; i++) {
      _pins[i] = sensorPins[i];
    }
  }

  // Initialize sensor
  void begin() {
    qtr.setTypeRC();
    qtr.setSensorPins(_pins, SensorCount);
  }

  void update() {
    // Get value from sensor
    qtr.read(_rawVal);

    // offset raw value
    for (int i = 0; i < SensorCount; i++){
      _unbiasedVal[i] = _rawVal[i] - sensorBias;
    }
  }

  void getValues(uint16_t values[SensorCount]) const {
    for (uint8_t i = 0; i < SensorCount; i++) {
      values[i] = _unbiasedVal[i];
    }
  }

  private:
    QTRSensors qtr;
    uint8_t _pins[SensorCount];
    uint16_t  _rawVal[SensorCount];
    uint16_t  _unbiasedVal[SensorCount];
};

#endif