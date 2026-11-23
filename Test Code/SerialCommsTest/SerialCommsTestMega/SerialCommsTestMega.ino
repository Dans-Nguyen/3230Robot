#include "SerialCommsMega.h"
SerialCommsMega SerialMega(Serial2, 115200); // Must match uno's baud rate of 115200

void setup() {
    Serial.begin(57600);
    Serial2.begin(115200);
    //SerialMega.begin();
}

void loop() {
    while (Serial2.available() > 0) {
        Serial.print(F("Byte: "));
        Serial.println(Serial2.read());
    }
}