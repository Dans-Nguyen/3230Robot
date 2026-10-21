// Library and Object Setup

#include <SoftwareSerial.h>
SoftwareSerial UnoXbee(2, 3);  // RX, TX

void playMessage(){
  Serial.println(F("Drive Motors:"));
  Serial.println(F("F = Forward"));
  Serial.println(F("B = Backward"));
  Serial.println(F("> = Rotate Clockwise"));
  Serial.println(F("< = Rotate Counter Clockwise"));

  Serial.println(F("RnP Motor:"));
  Serial.println(F("E = Extend "));
  Serial.println(F("R = Retract"));

  Serial.println(F("Servos:"));
  Serial.println(F("A = Arm servo"));
  Serial.println(F("P = Pick button servo"));
  Serial.println(F("D = Paddle servo"));
  Serial.println(F("H = Ship button servo"));
  Serial.println(F("L = Lever servo"));
}

void setup() {
  // put your setup code here, to run once:
  // Open serial communications with computer and wait for port to open:
  Serial.begin(57600); // make sure to also select this baud rate in your Serial Monitor window

  // 9600 is reliable with SoftwareSerial on an Uno. The Mega must use the
  // same rate on Serial1, and both XBees must be configured for 9600 baud.
  UnoXbee.begin(115200);

  Serial.println(F("UNO remote ready"));

  playMessage();

}

char command = "a";


void loop() {

  // Did user type something into laptop?
  if (Serial.available() > 0) {
    char command = Serial.read();

    // Send starting flag val
    UnoXbee.write(255);

    // Send that command wirelessly
    UnoXbee.print(command);

    Serial.print(F("Sent to robot: "));
    Serial.println(command);

    command = 'a';
    playMessage();
  }


}
