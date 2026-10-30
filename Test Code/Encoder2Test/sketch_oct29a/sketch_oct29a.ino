#include <Encoder.h>

Encoder myEnc2(20, 21);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(myEnc2.read());
  delay(100);
}