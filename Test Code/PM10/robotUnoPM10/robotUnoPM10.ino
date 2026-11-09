// Library and Object Setup

#include <SoftwareSerial.h>
SoftwareSerial UnoXbee(2, 3);  // RX, TX

void playMessage(){
  Serial.println(F("Command for Trajectory:"));
  Serial.print(F("Straight:"));
  Serial.print("\t");
  Serial.println(F("1 [dist (cm)] [time (s)]"));
  Serial.print(F("Rotate:"));
  Serial.print("\t");
  Serial.print("\t");
  Serial.println(F("2 [degrees] [time (s)]"));
  Serial.print(F("Curve:"));
  Serial.print("\t");
  Serial.print("\t");
  Serial.println(F("3 [degrees] [radius (cm)] [time (s)]"));

  
}

void setup() {
  // put your setup code here, to run once:
  // Open serial communications with computer and wait for port to open:
  Serial.begin(57600); // make sure to also select this baud rate in your Serial Monitor window

  UnoXbee.begin(115200); // make sure this baud rate matches with Serial2 baud rate on mega

  Serial.println(F("UNO remote ready"));

  playMessage();

}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.length() == 0) return;

    UnoXbee.write((uint8_t)255);  // start-of-command marker
    UnoXbee.println(command);     // sends the entire command plus newline

    Serial.print(F("Sent to robot: "));
    Serial.println(command);

    playMessage();
  }
}
