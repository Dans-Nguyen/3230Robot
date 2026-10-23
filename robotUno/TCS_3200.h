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
LEDPin = 13;
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
  // sets up our sensor pins
  MD_TCS3200(uint8_t sensorPin s0Pin, uint8_t sensorPin s1Pin, uint8_t sensorPin s2Pin, uint8_t sensorPin S3pin,
             uint8_t sensorPin outPin, uint8_t sensorPin OEPin)

  // get inital reading

  void begin() {  // what do we put here ?
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(readPin, INPUT);
    pinMode(LEDPin, OUTPUT);
    digitalWrite(s0, HIGH);  // s1 and s0 choose frequency scaling
    digitalWrite(s1, LOW);

    digitalWrite(LEDPin, HIGH);  //turn on LED
    Serial.println("Type in anything to the serial monitor to get a values");
    Serial.println("Red\t\tGreen\t\tBlue\t\tClear");
  }



// Start the color sensor at zero
//_colorFreq = 0;

// This is it's own section seperate ---- takes readings from the sensor
//Blue Filter lab code
void readColors() {
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
} // void ReadColors 
void movingAverage (float * arr) {
float sum = 0;
for (int i = 0; < numSamples; i++) {
  sum += arr[i]/numSamples;
}
return sum;
}


private:
// Sensor pin variable
uint8_t _s0;
uint8_t _s1;
uint8_t _s2;
uint8_t _s3;
uint8_t _readPin;  // OUT?
uint8_t _LEDPin;   // OE ?

//float _colorFreq;
float _R;
float _G;
float _B;
float _C;

float _RF;
float _GF;
float _BF;
float _CF;

const int _numSamples;
I
}
;

#endif