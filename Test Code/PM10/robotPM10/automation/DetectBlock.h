#ifndef DETECT_BLOCK_H
#define DETECT_BLOCK_H

class DetectBlock {
public:

  DetectBlock(MD_TCS3200& colorSensor, Hall_Effect& hallEffect)
  : _colorSensor(colorSensor), _hallEffect(hallEffect), _blockColor('N'), _Volt(0.0f), _isMagnet(false) {}

  void update(){
    _colorSensor.update();
    _Volt = _hallEffect.getVal();

    _blockColor = _colorSensor.getColor();

    // Check if quiescent voltage is some value above or below 2.5V
    if (_Volt >= 3.0f || _Volt <= 2.0f) {
      _isMagnet = true;
    } else {
      _isMagnet = false;
    }

  }

  char getColor () {
    return _blockColor;
  }

  bool isMagnet () {
    return _isMagnet;
  }

private:
MD_TCS3200& _colorSensor;
Hall_Effect& _hallEffect;

char _blockColor;
float _Volt;
bool _isMagnet;

};

#endif