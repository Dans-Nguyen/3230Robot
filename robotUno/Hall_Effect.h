/* 
Hall Effect Sensor Dectects if there was a magnet block that passed by the sensor

Mega Pin : A2 
Uno  Pin : A2

*/

class Hall_Effect {
public:
  //Hall_Effect(uint8_t sensorPin ) // not sure about this
  _hallEffect_pin = sensorPin;

  void begin() {
    pinmode(_hallEffect_pin, INPUT);
  }
  void readMagnet() {
    // quiescent value is 2.5V
    // max is 5V
    
  }
  void 
private:
  int _hallEffectPin
}
;
#endif