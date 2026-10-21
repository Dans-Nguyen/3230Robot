#include <SoftwareSerial.h>
SoftwareSerial UnoXbee(2, 3);  // RX, TX

char command;
int state = 0;

void setup() {
  // put your setup code here, to run once:
  // Open serial communications with computer and wait for port to open:
  Serial.begin(57600); // make sure to also select this baud rate in your Serial Monitor window

  // 9600 is reliable with SoftwareSerial on an Uno. The Mega must use the
  // same rate on Serial1, and both XBees must be configured for 9600 baud.
  UnoXbee.begin(115200);

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

}
