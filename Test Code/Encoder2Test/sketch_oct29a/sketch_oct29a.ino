#include <Encoder.h>

Encoder myEnc2(3, 22);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(myEnc2.read());
  delay(100);
}