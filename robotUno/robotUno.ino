// Compoents On Uno - Hall Effect, Color

#include "MD_TCS_3200.h" // Color
#include "Hall_Effect.h" // Magnet

// Automation classes
#include "DetectBlock.h" // block color and magnet
#include "SerialCommsUno.h"
#include <SoftwareSerial.h>

SoftwareSerial UnoXbee(2, 3);
MD_TCS3200 colorSensor(4, 5, 6, 7, 8, 10);
Hall_Effect hallEffect(A2);

DetectBlock detectBlock(colorSensor, hallEffect);
SerialCommsUno SerialUno(UnoXbee, 115200UL); // Must match Mega's Serial2 baud rate of 115200

int state = 0; // Internal state
int stateSend = 0; // State sent to mega
uint8_t activeOrder[SerialCommsUno::MAX_ARRAY_SIZE];


void setup() {
  Serial.begin(57600); 
  SerialUno.begin(); 

  // Begin component objects
  hallEffect.begin();
  colorSensor.begin();

  Serial.println(F("UNO remote ready"));

}


void loop() {
  // Check for serial comm
  SerialUno.update();

  if (SerialUno.stateAvailable) {
    state = SerialUno.getState();
  } else if (SerialUno.arrayAvailable){
    const uint8_t* receivedOrder = SerialUno.getArray();

    for (uint8_t i = 0; i < SerialCommsUno::MAX_ARRAY_SIZE; i++) {
        activeOrder[i] = receivedOrder[i];
    }
  }

  // Set state manually from Serial
  if (Serial.available() > 0) {
    int requestedState = Serial.parseInt();

    if (requestedState != 999) {
        state = requestedState;
    }

    while (Serial.available() > 0) {
        Serial.read();
    }
  }
  //Serial.println(state);

  state = TestCasesUno(state);
}