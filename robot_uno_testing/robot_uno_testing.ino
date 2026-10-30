// Compoents On Uno - Hall Effect, Color

#include "components/MD_TCS_3200.h" // Color
#include "components/Hall_Effect.h" // Magnet



// Automation classes
// #include "automation/ServoMovement" //change when we have this done
#include "automation/DetectBlock.h"  // block color and magnet
#include "automation/ReceiveOrder.h" // Getting from kiosk


#include <SoftwareSerial.h>
SoftwareSerial UnoXbee(2, 3);  // RX, TX
MD_TCS3200 colorSensor(4, 5, 6, 7, 8,10); // (S0,S1,S2,S3, Out, LED)
Hall_Effect hallEffect(A2);


// automation calls (active order storage variable)

DetectBlock detectBlock(colorSensor, hallEffect);
ReceiveOrder::BlockColor activeOrder[ReceiveOrder::MAX_BLOCKS];
uint8_t activeOrderCount = 0;


char command;
int state = 0;

void setup() {
 // setUp
 // Open serial communications with computer and wait for port to open:
 Serial.begin(57600); // make sure to also select this baud rate in your Serial Monitor window
  // 9600 is reliable with SoftwareSerial on an Uno. The Mega must use the
 // same rate on Serial1, and both XBees must be configured for 9600 baud.
  UnoXbee.begin(9600);
 colorSensor.begin(); // start the color

 Serial.println(F("UNO remote ready"));
 Serial.println(F("S = Start Robot"));


}


void loop() {
// Set state directly from serial monitor. For debuggin purposes
 if (Serial.available() > 0) {
   int requestedState = Serial.parseInt();


   if (requestedState != 0) {
     state = requestedState;


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


     Serial.print(F("Sent to Percy: "));
     Serial.println(state);
   }
switch (state) {
 case 120: { // Order updates, case number is the same as robot
    if (order.update()) {
       activeOrderCount = order.count();


       // A complete start → colors → end message was received.
       for (uint8_t i = 0; i < order.count(); i++) {
         activeOrder[i] = order.block(i); // store block color to activeOrder


         Serial.println(activeORder[i])


       }
     }




 }
 case 121 : { // Detect what color and if theres a magnet
detectBlock.update();
     char blockColor = detectBlock.getColor();
     bool isMagnet = detectBlock.isMagnet();
  
     Serial.print(blockColor);
     Serial.println(isMagnet);


 break; }
 case 130 : { // Servos, change number once we decide on Mega

break; }

 }
}
  
 }
    }
}
}
