

SharpGP2Y0A21YK0F distSensorF(A4, 0.2);
//QTRMD08RC ...

int state = 0;
void setup() {
  // put your setup code here, to run once:
distSensorF.begin();
Serial.begin(9600);



void loop() {
  // put your main code here, to run repeatedly:
  float distCm = distSensorF.getDistCm();
  Serial.println(distCm);
  switch (state) {

    case 1: // state for line following//

      Serial.println("line following");
      // line follow code
      lineFollow.begin();
        /*   drive forward
        if arrays on the left are showing black lines
        right motor speed increases
        if arrays on right are showing black lines
        left motor speed increases */ 
        
      if(distCm <= 38);
      // distance from wall achieved, stop line following
      state = 2;
      break;
    case 2: // state for turning 90 degrees

      Serial.println("Turning")
      // after time elapsed (turned 90 degrees)

    break;

    case 3: // state for wall following

      Serial.println("Wall following");
      // after time elapsed (ginny run into wall)
      // state = 4;

    break;

    case 4: // state for reversing

    Serial.println("Reversing");
    // if count from line counting = 2
    // state = 5;
    


  

    




  }
}
