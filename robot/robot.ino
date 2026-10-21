// Component classes
#include "components\SharpGP2Y0A21YK0F.h"
#include "components\QTRMD08RC.h"

// Automation classes
#include "automation\DriveControl.h"
#include "automation\LineFollow.h"

DriveControl drive;
SharpGP2Y0A21YK0F distSensorF(A4, 0.2);
SharpGP2Y0A21YK0F distSensorR(A5, 0.2);
QTRMD08RC lineSensor({37, 36, 39, 38, 41, 40, 43, 42});
LineFollow lineFollow(lineSensor, drive);

double t, t0=0; // declare some time variables

// Initialize state variables
int state = 0;
char stateChar = "A";

void setup() {
  drive.begin();
  distSensorF.begin();
  lineSensor.begin();

  t0 = micros()/1000000.; // initialize time
}

void loop() {
  
  switch (state) {

    case 1: // state for line following//
      lineFollow.start(50, 50);

      if(distSensorF.getDistCm() <= 38){
        // distance from wall and front sensor achieved, stop line following
        drive.stop();
        state = 2;
      }
      break;

    case 2: // state for turning 90 degrees
      drive.turnCCW(100);
      
      if(distSensorR.getDistCm() <= 38){
        // distance from wall and right sensor achieved, stop rotating
        drive.stop();
        state = 3;
      }
    break;

    case 3: // state for wall following

      
      // after time elapsed (ginny run into wall)
      // state = 4;

    break;

    case 4: // state for reversing

    
    // if count from line counting = 2
    // state = 5;
    break;
  
  }


}
