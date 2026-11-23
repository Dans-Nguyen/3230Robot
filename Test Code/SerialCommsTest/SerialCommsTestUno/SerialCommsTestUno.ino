#include "SerialCommsUno.h"
#include <SoftwareSerial.h>


SoftwareSerial UnoXbee(2, 3);  // RX, TX
//SerialCommsUno SerialUno(UnoXbee, 115200); // Must match Mega's Serial2 baud rate of 115200


void setup() {
    Serial.begin(57600);
    UnoXbee.begin(115200);
    //SerialUno.begin();
}

void loop() {
    UnoXbee.write((uint8_t)77);
    Serial.println("State Sent");
    delay(1000);
}