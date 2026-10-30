// Compoents On Uno - Hall Effect, Color

// Componet classes - calling from componets class to interact with object we made
#include "components/MD_TCS_3200.h"  // Color Sensor
#include "components/Hall_Effect.h"  // detect magnets


// Automation classes - accomplish task from object
// #include "automation/ServoMovement" //change when we have this done
#include "automation/DetectBlock.h"   // block color and magnet
#include "automation/ReceiveOrder.h"  // Getting from kiosk


#include <SoftwareSerial.h>
SoftwareSerial UnoXbee(2, 3);               // RX, TX
MD_TCS3200 colorSensor(4, 5, 6, 7, 8, 10);  // (S0,S1,S2,S3, Out, LED)
Hall_Effect hallEffect(A2);


// automation calls (active order storage variable)
// class (blue print) --> object (constuctor) (function in class) (call pins or functions from library created)

DetectBlock detectBlock(colorSensor, hallEffect);
ReceiveOrder::BlockColor activeOrder[ReceiveOrder::MAX_BLOCKS];
uint8_t activeOrderCount = 0;

char command;
int state = 0;

void setup() {
  // setUp wont return numerical values
  // Open serial communications with computer and wait for port to open:
  // 9600 is reliable with SoftwareSerial on an Uno. The Mega must use the
  // same rate on Serial1, and both XBees must be configured for 9600 baud.

  Serial.begin(57600);  // make sure to also select this baud rate in your Serial Monitor window
  UnoXbee.begin(9600);
  colorSensor.begin();  // start the color sensor

  Serial.println(F("UNO remote ready"));
  Serial.println(F("S = Start Robot"));
}


void loop() {

  // check for Mega commands (Servo (ship command) and (flip switch) and kiosk order )
  if (UnoXbee.available() >= 2 && UnoXbee.read() == 255) {
    state = UnoXbee.read();
    Serial.print(F("Uno received : "));
    Serial.print(state);
  }

  // Set state directly from serial monitor. For debuggin' purposes
  if (Serial.available() > 0) {

    // Did user type something into laptop?
    command = Serial.read();
    if (command == 'S') {
      Serial.read();
      state = 1;
      // Send starting flag val
      UnoXbee.write(255);

      // Send that command wirelessly
      UnoXbee.write(state);
      Serial.println("Start");
      Serial.print(F("Sent to Percy: "));  // F - flash memory based strings - free up memory
      Serial.println(state);
    } 
    else {
      int requestedState = Serial.parseInt();  // parseInt -- reads the next valid interger from the serial data
      if (requestedState != 0) {               // if valid long int found
        state = requestedState;                // return the int or not valid int will return 0
        Serial.print(F("State set to: "));
        Serial.println(state);
      }
      while (Serial.available() > 0) {
        Serial.read();
      }
    }
  }
  //if (UnoXbee.available() > 0) {

  // }  //belongs to --- > UnoXbee.available() > 0)
  switch (state) {
    case 120: {  // Order updates, case number is the same as robot
        if (order.update()) {
          activeOrderCount = order.count();

          // A complete start → colors → end message was received.
          for (uint8_t i = 0; i < order.count(); i++) {
            activeOrder[i] = order.block(i);  // store block color to activeOrder

            Serial.println(activeORder[i]);
          }
        }
      }
    case 121: {  // Detect what color and if theres a magnet
        detectBlock.update();
        char blockColor = detectBlock.getColor();
        bool isMagnet = detectBlock.isMagnet();
        Serial.print(F("Block color : "));
        Serial.print(blockColor);
        Serial.print(F("Magnet state : "));
        Serial.println(isMagnet);

        break;
      }
    case 130: {  // Servos, change number once we decide on Mega

        break;
      }

  }  // switch statement
}  // this should be the end of void loop
