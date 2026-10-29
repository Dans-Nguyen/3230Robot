// Component classes
#include "components\SharpGP2Y0A21YK0F.h"
#include "components\QTRMD08RC.h"
#include "components\MD_TCS_3200.h"
#include "components\Hall_Effect.h"
#include "components\LTR_4206E_IR.h"

// Automation classes
#include "automation\DriveControl.h"
#include "automation\LineFollow.h"
#include "automation\DetectBlock.h"
#include "automation\ReceiveOrder.h"

DriveControl drive;
SharpGP2Y0A21YK0F distSensorF(A4, 0.2);
SharpGP2Y0A21YK0F distSensorR(A5, 0.2);
QTRMD08RC lineSensor({37, 36, 39, 38, 41, 40, 43, 42});
LineFollow lineFollow(lineSensor, drive);
MD_TCS3200 colorSensor(27, 28, 29, 30, 31, 26);
Hall_Effect hallEffect(A2);
DetectBlock detectBlock(colorSensor, hallEffect);
LTR_4206E_IR IR(25);
ReceiveOrder order(IR, 10.0f);
 

double t, t0, time=0; // declare some time variables

// Initialize data storage variables
float distCm; // Distance from distance sensor in cm

// Active order storage variable
ReceiveOrder::BlockColor activeOrder[ReceiveOrder::MAX_BLOCKS];
uint8_t activeOrderCount = 0;

// Initialize state variables
int state = 0;

void setup() {
  Serial.begin(57600);      // for Serial Monitor messages
  Serial2.begin(115200);    // communication with the XBee / Uno

  // begin componenet objects
  drive.begin();
  distSensorF.begin();
  lineSensor.begin();
  colorSensor.begin();
  IR.begin();
  IR.setTimeout(8000); // Timeout is longer than slowest signal (3333 micros) 
                       // and shorter than LED-off gap (100000 micros)

  t0 = micros()/1000000.; // initialize time in seconds
}

void loop() {
  
  // Check for uno command
  if (Serial2.available() >= 2 && Serial2.read() == 255) {
    state = Serial2.read();

    Serial.print(F("Mega received: "));
    Serial.println(state);
  }

  // Set state directly from serial monitor. For debuggin purposes
  if (Serial.available() > 0) {
    int requestedState = Serial.parseInt();

    if (requestedState != 0) {
      state = requestedState;

      Serial.print(F("State set to: "));
      Serial.println(state);
    }

    // Remove the newline / carriage-return left by Serial Monitor.
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  switch (state) {

    case 1:{ // state for line following//
      lineFollow.start(150, 100);

      // Get distance data from front sensor
      distCm = distSensorF.getDistCm();

      if(distCm <= 38){
        // distance from wall and front sensor achieved, stop line following
        drive.stop();
        state = 2;
      }

    break;}

    case 2:{ // state for turning 90 degrees
      drive.turnCCW(100);
      
      // Get distance data from right sensor
      distCm = distSensorR.getDistCm();

      if((distCm <= 38) && (distCm >= 36)){
        // distance from wall and right sensor achieved, stop rotating
        drive.stop();
        state = 3;
      }

    break;}

    case 3:{ // state for wall following

      
      // after time elapsed (ginny run into wall)
      // state = 4;

    break;}

    case 4:{ // state for reversing

    
    // if count from line counting = 2
    // state = 5;
    break;}

    // 120th state for testing order reception
    case 120:{
      if (order.update()) {
        activeOrderCount = order.count();

        // A complete start → colors → end message was received.
        for (uint8_t i = 0; i < order.count(); i++) {
          activeOrder[i] = order.block(i); // store block color to activeOrder

          Serial.println(activeORder[i])

        }
      }

      state = 999;
    break;}

    // 121st state for testing block detection
    case 121:{
      detectBlock.update();
      char blockColor = detectBlock.getColor();
      bool isMagnet = detectBlock.isMagnet();
      Serial.print(blockColor);
      Serial.println(isMagnet);
      
    break;}
  
  }
}
