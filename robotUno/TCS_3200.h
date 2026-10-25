/* 
Color Sensor Class for the TCS3200 

Reads the color from reflected from the block as raw data and to turn into our frequency 1/data to get frequency
Pins (Uno):
Pin 4 (S0 ) pinMode(s0,OUTPUT)
Pin 5 (S1 ) pinMode(s1,OUTPUT)
Pin 6 (S2 ) pinMode(s2,OUPUT)
Pin 7 (S3 ) pinMode(s3,OUTPUT)
Pin 8 (Out) 
Pin 9 (OE )
Pin 10(LED)

Pins (Mega): 
Pin 25 (S0 ) pinMode(s0,OUTPUT)
Pin 26 (S1 ) pinMode(s1,OUTPUT)
Pin 27 (S2 ) pinMode(s2,OUPUT)
Pin 28 (S3 ) pinMode(s3,OUTPUT)
Pin 29 (Out) 
Pin 30 (OE )
Pin 31 (LED)

Useage in Lab Code : 
MD_TCS3200 colorSensor(4,5,6,7,8,9);

// In Set up 

colorSensor.begin ();

// In loop

 digitalWrite(s2,LOW);

*/

#indef MD_TCS3200_H
#define MD_TCS3200_H
// include <Arduino.h>
// include <MD_TCS3200.h>
class MD_TCS3200 {
public:
  // construtor
  // sets up our sensor pins
  MD_TCS3200(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3,
             uint8_t outPin, uint8_t OEPin, uint8_t LEDPin)  // might not have the word pin might just be s0
//
  // get inital reading

  void begin() {  // what do we put here ?
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(OutPin, INPUT);
    pinMode(LEDPin, OUTPUT);
    pinMode(OEPin, OUTPUT);
    
    digitalWrite(s0, HIGH);  // s1 and s0 choose frequency scaling
    digitalWrite(s1, LOW);
    digitalWrite(OEPin_LOW); // pulldown i think
    digitalWrite(LEDPin, HIGH);  //turn on LED
    Serial.println("Type in anything to the serial monitor to get a values");
    Serial.println("Red\t\tGreen\t\tBlue\t\tClear");
  }
unsisigned long readPulse ( ){
  return pulseIn(OutPin,LOW) //
}


  // Start the color sensor at zero
  //_colorFreq = 0;

  // This is it's own section seperate ---- takes readings from the sensor
  
  void readColors() {
    //Blue Filter lab code
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);

    for (int i = 0; i < numSamples; i++) {
      B[i] = readPulse();
    }
    //Red Filter
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);

    for (int i = 0; i < numSamples; i++) {
      R[i] = readPulse();
    }
    //Green Filter
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);

    for (int i = 0; i < numSamples; i++) {
      G[i] = readPulse();
    }
    // CLEAR Filter
    digitalWrite(s2, HIGH);
    digitalWrite(s3, LOW);

    for (int i = 0; i < numSamples; i++) {
      C[i] = readPulse();
    }
  }  // void ReadColors
  void movingAverage(float* arr) { // float movingAverage ? come back to
    float blockColor_sum = 0;
    for (int i = 0; < numSamples; i++) {
      blockColor_sum += arr[i] / numSamples;
    }
    return blockColorsum;
  }

  void blockColors() {
    readColors ();
    float avgR = movingAverage(R);
    float avgG = movingAverage(G);
    float avgB = movingAverage(B);
    // Algorithm for deciding what color a block is
    // Blue filter, red filter, green filter in order... Normalized sensor range
    // Blue block is colors [49.1,55.66], [14.84,19.6], [30.4,31.46]
    // Red block is [21.51,21.88], [55.68,58.82], [16.94,18.35]
    // Yellow block is [21.46,23.08], [36.72,39.06], [35,37.94]
    // Brown block is [ ]
    // Decide if we want block or if we want to push block based on what color is read (within range)
    if (blockColorSum) { // so i think this should be if avgR is in some range of && avG && avgB
                         // return red
      // Red block
      blockColor = red;


      else if {
        // blue block
        blockColor = blue;
      }


      else if {
        // yellow block
        blockColor = yellow;
      }
      else {
        // brown block
        blockColor = brown;
      }
    }
  }


private:
  // Sensor pin variable
  uint8_t _s0;
  uint8_t _s1;
  uint8_t _s2;
  uint8_t _s3;
  uint8_t _OutPin;  // OUT?
  uint8_t _LEDPin;  
  uint8_t _OEpin //OE filter

  /* not using these right now 
  float _colorFreq;
  float _R;
  float _G;
  float _B;
  float _C;

  // Filtered values
  float _RF;
  float _GF;
  float _BF;
  float _CF;
*/
  const int _numSamples;
};

#endif