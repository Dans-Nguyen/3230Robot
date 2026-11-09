/*
Distance Sensor Class for the Sharp GP2Y0A21YK0F 10-80 cm distance sensor.

Reads analog voltage from the IR sensor, applies an IIR filter, and converts
filtered voltage into distance in cm using a user calibrated function. Check 
the 'Sensor Calibration Data\' folder in the repository for distance sensor
data and calibration.

Typical usage:

SharpGP2Y0A21YK0F distSensorF(A4, 0.2);    // Pin A4. alpha = 0.2

// In setup
distSensorF.begin();

// In loop
float distCm = distSensorF.getDistCm();  // Get distance in cm

*/

#ifndef SharpGP2Y0A21YK0F_H
#define SharpGP2Y0A21YK0F_H

class SharpGP2Y0A21YK0F {
  public:
    SharpGP2Y0A21YK0F(uint8_t sensorPin, float alpha = 0.1f)
    : _pin(sensorPin), _alpha(constrain(alpha, 0.0f, 1.0f)),
    _rawVal(0), _rawVolt(0.0f), _filteredVolt(0.0f), _distCm(0.0f) {}

    // Get initial reading
    void begin() {
      _rawVal = analogRead(_pin);
      _rawVolt = adcToVoltRaw(_rawVal);

      // Start the filter at the first reading—no ramp from zero.
      _filteredVolt = _rawVolt;
      _distCm = 0;
    }

    // Convert raw ADC to voltage
    float adcToVoltRaw(int rawADC) const {
      return rawADC * (5.0f / 1023.0f);
    }

    // Set IIR filter weight
    void setAlpha(float alpha) {
      _alpha = constrain(alpha, 0.0f, 1.0f);
    }

    // Get a reading from dist sensor, filter using IIR, 
    // and convert filtered voltage to distance
    float getDistCm() {
      // Read raw voltage
      _rawVal = analogRead(_pin);

      // Convert raw ADC to raw voltage
      _rawVolt = adcToVoltRaw(_rawVal);

      // Filter voltage
      _filteredVolt = _alpha * _rawVolt + (1.0f - _alpha) * _filteredVolt;

      // Convert voltage to distance
      _distCm = exp((log(_filteredVolt) - 2.851f) / -0.8776f);

      return _distCm;
    }

  private:
      uint8_t _pin;
      float _alpha;

      int _rawVal;
      float _rawVolt;
      float _filteredVolt;
      float _distCm;
};

#endif