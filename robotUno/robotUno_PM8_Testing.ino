// Compoents On Uno 
// Hall Effect and TCS3200 Color Sensor
#include "components/TCS3200.h"
//#include "components/Hall_Effect.h"


#include <SoftwareSerial.h>
SoftwareSerial UnoXbee(2, 3);  // RX, TX
MD_TCS3200 colorSensor(4, 5, 6, 7, 8,9,10);
Hall_Effect hallSensor(A2);
// const int hallPin = A2 ??

char command;
int state = 0;

void setup() {
  // put your setup code here, to run once:
  // Open serial communications with computer and wait for port to open:
  Serial.begin(57600); // make sure to also select this baud rate in your Serial Monitor window

  // 9600 is reliable with SoftwareSerial on an Uno. The Mega must use the
  // same rate on Serial1, and both XBees must be configured for 9600 baud.
  UnoXbee.begin(9600);
  // colorSensor.begin () : // color sensor fucntion 
  //hallSensor.begin
  //pinMode(hallPin, INPUT)

  Serial.println(F("UNO remote ready"));
  Serial.println(F("S = Start Robot"));

}

void loop() {
  // Did user type something into laptop?
  if (Serial.available() > 0) {
    command = Serial.read();

    if (command == 'S'){
      Serial.println("Start");
      state = 1;
      // Send starting flag val
      UnoXbee.write(255);

      // Send that command wirelessly
      UnoXbee.write(state);

      Serial.print(F("Sent to robot: "));
      Serial.println(state);
    }

    command = 'a';
    state = 0;
  }
  /*
  // This is what i am thinking so far Hall Effect pin
if (state == 1) {
  int hallValue = analogRead(hallPin);
  byte scaledHall = map(hallValue, 0, 1023, 0, 255)
// Color
// Block Color from class
    BlockColor detectedBlock = colorSensor.getColor(); // this is depend on class
    
    Serial.print(F("Hall (A2): "));
    Serial.print(hallValue);
    Serial.print(F(" | Block Color: "));
    Serial.println(colorSensor.colorToString(detectedBlock));

    UnoXbee.write(scaledHall);
    UnoXbee.write((byte)detectedBlock);
colorSensor.readColors ();

  }

  

  */
 
 }
}
