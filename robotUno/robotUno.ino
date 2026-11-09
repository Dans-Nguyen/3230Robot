// Compoents On Uno - Hall Effect, Color

#include "MD_TCS_3200.h" // Color
#include "Hall_Effect.h" // Magnet

// Automation classes
#include "DetectBlock.h" // block color and magnet


#include <SoftwareSerial.h>
SoftwareSerial UnoXbee(2, 3);  // RX, TX
MD_TCS3200 colorSensor(4, 5, 6, 7, 8,10); // (S0,S1,S2,S3, Out, LED)
Hall_Effect hallEffect(A2);

DetectBlock detectBlock(colorSensor, hallEffect);

char command;
int state = 0;

void setup() {
  Serial.begin(57600); 
  UnoXbee.begin(115200); // Must match Mega's Serial2 baud rate of 115200

  // Begin component objects
  hallEffect.begin();
  colorSensor.begin();

 Serial.println(F("UNO remote ready"));
 Serial.println(F("S = Start Robot"));


}


void loop() {


 // Set state directly from serial monitor. For debuggin purposes
  if (Serial.available() > 0) {
    int requestedState = Serial.parseInt();

    if (requestedState != 0) {
      state = requestedState;
   
    }

    command = Serial.read();
    
    if (command == 'S'){
      Serial.println("Start");
      state = 1;
      // Send starting flag val
      UnoXbee.write(255);

      // Send that command wirelessly
      UnoXbee.write(state);

      Serial.print(F("Sent to Percy: "));
      Serial.println(state);
    }
  }

  switch (state) {

    case 121 : { // Detect what color and if theres a magnet
      detectBlock.update();
      char blockColor = detectBlock.getColor();
      bool isMagnet = detectBlock.isMagnet();
  
      Serial.print(blockColor);
      Serial.println(isMagnet);

      Serial.print("Color: ");
      Serial.print(blockColor);
      Serial.print(" | Magnet: ");
      Serial.println(isMagnet);

    break;}
  
  }
}

