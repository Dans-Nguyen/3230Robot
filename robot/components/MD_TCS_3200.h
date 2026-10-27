/* 
Color Sensor Class for the TCS3200 

Reads the color from reflected from the block as raw data

Pins (Uno):
Pin 4 (S0 ) pinMode(s0,OUTPUT)
Pin 5 (S1 ) pinMode(s1,OUTPUT)
Pin 6 (S2 ) pinMode(s2,OUPUT)
Pin 7 (S3 ) pinMode(s3,OUTPUT)
Pin 8 (Out) 
Pin 10(LED)

Pins (Mega): 
Pin 27 (S0 ) pinMode(s0,OUTPUT)
Pin 28 (S1 ) pinMode(s1,OUTPUT)
Pin 29 (S2 ) pinMode(s2,OUPUT)
Pin 30 (S3 ) pinMode(s3,OUTPUT)
Pin 31 (Out) 
Pin 26 (LED)

Useage in Lab Code : 
MD_TCS3200 colorSensor(4,5,6,7,8,9);

// In Set up 

colorSensor.begin();

// In loop
colorSensor.update();
char color = colorSensor.getBlockColor();


*/

#ifndef MD_TCS3200_H
#define MD_TCS3200_H

class MD_TCS3200 {
public:
  // Set up pins
  MD_TCS3200(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t outPin, uint8_t LEDPin, uint8_t numSamples = 5)
      : _s0(s0), _s1(s1), _s2(s2), _s3(s3), _outPin(outPin), _LEDPin(LEDPin), _numSamples(numSamples) {}

  void begin() {
    pinMode(_s0, OUTPUT);
    pinMode(_s1, OUTPUT);
    pinMode(_s2, OUTPUT);
    pinMode(_s3, OUTPUT);
    pinMode(_outPin, INPUT);
    pinMode(_LEDPin, OUTPUT);
    
    digitalWrite(_s0, HIGH);  // s1 and s0 choose frequency scaling
    digitalWrite(_s1, LOW);
    digitalWrite(_LEDPin, HIGH);  //turn on LED
  }

  unsigned long readPulse ( ){
    return pulseIn(_outPin, LOW, 30000UL) + 
           pulseIn(_outPin, HIGH, 30000UL);
  }
  
  float averageFilter(bool s2State, bool s3State) {
    // Collects and filters data using averaging filter
    digitalWrite(_s2, s2State);
    digitalWrite(_s3, s3State);
    delay(2);  // let the selected filter settle

    unsigned long sum = 0;
    for (uint8_t i = 0; i < _numSamples; i++) {
      sum += readPulse();
    }
    return (float)sum / _numSamples;
  } 

  void update() {

    // Collect data
    _Rf = averageFilter(LOW, LOW);
    _Gf = averageFilter(HIGH, HIGH);
    _Bf = averageFilter(LOW, HIGH);
    _Cf = averageFilter(HIGH, LOW);

    // Serial.print(_Rf, 2);
    // Serial.print(",\t");
    // Serial.print(_Gf, 2);
    // Serial.print(",\t");
    // Serial.print(_Bf, 2);
    // Serial.print(",\t");
    // Serial.println(_Cf, 2);

    if (_Cf <= 0) {
      _blockColor = 'N';   // invalid/no reading
      return;
    }

    // Normalize color data
    float RNorm = _Cf/_Rf * 100.0f;
    float GNorm = _Cf/_Gf * 100.0f;
    float BNorm = _Cf/_Bf * 100.0f;

    // Serial.print(RNorm, 2);
    // Serial.print(",\t");
    // Serial.print(GNorm, 2);
    // Serial.print(",\t");
    // Serial.println(BNorm, 2);

    if (BNorm >= 20 && BNorm <= 25 &&
        RNorm  >= 60 && RNorm  <= 80 &&
        GNorm >= 15 && GNorm <= 20){

      // Red block
      _blockColor = 'R';

    } else if (BNorm >= 40 && BNorm <= 60 &&
               RNorm >= 10 && RNorm <= 25 &&
               GNorm >= 20 && GNorm <= 34){
                
      // blue block
      _blockColor = 'B';

    } else if(BNorm >= 20 && BNorm <= 25 &&
              RNorm >= 40 && RNorm <= 60 &&
              GNorm >= 30 && GNorm <= 40){

      // yellow block
      _blockColor = 'Y';

    } else {
      // brown block
      _blockColor = 'K';
    }

    

  }

  char getColor() const {
    return _blockColor;
  }

private:
  // Sensor pin variable
  uint8_t _s0;
  uint8_t _s1;
  uint8_t _s2;
  uint8_t _s3;
  uint8_t _outPin;
  uint8_t _LEDPin;  

  // Filtered values
  float _Rf;
  float _Gf;
  float _Bf;
  float _Cf;

  // Block Color
  char _blockColor;
  
  const int _numSamples;
};

#endif