// Component classes
#include "components\SharpGP2Y0A21YK0F.h"
#include "components\QTRMD08RC.h"
#include "components\MD_TCS_3200.h"
#include "components\Hall_Effect.h"
#include "components\LTR_4206E_IR.h"
#include <Encoder.h>

// Automation classes
#include "automation\PID.h"
#include "automation\DriveControl.h"
#include "automation\LineFollow.h"
#include "automation\DetectBlock.h"
#include "automation\ReceiveOrder.h"
#include "automation\DriveTrajectory.h"

// Initialize PID objects
PID lineFollowPID(100, 0, 0);  // PID object for line following
PID driveTraj1PID(100, 0, 10); // PID object for wheel 1 drive trajectory
PID driveTraj2PID(100, 0, 10); // PID object for wheel 2 drive trajectory

// Initialize Objects
DriveControl drive;
SharpGP2Y0A21YK0F distSensorF(A4, 0.2);
SharpGP2Y0A21YK0F distSensorR(A5, 0.2);
QTRMD08RC lineSensor({37, 36, 39, 38, 41, 40, 43, 42});
LineFollow lineFollow(lineSensor, drive, lineFollowPID);
MD_TCS3200 colorSensor(27, 28, 29, 30, 31, 26);
Hall_Effect hallEffect(A2);
DetectBlock detectBlock(colorSensor, hallEffect);
LTR_4206E_IR IR(25);
ReceiveOrder order(IR, 10.0f);
Encoder enc1(19, 18);
Encoder enc2(20, 21);

// Drive motor physical Parameters
double GR = 50;         // gear ratio
int countsPerRev = 64;  // encoder counts per Rev
double rw = 4.2;        // wheel radius in cm
double D = 21.7;        // distance between wheels in cm

DriveTrajectory driveTrajectory(GR, countsPerRev, rw, D, drive, 
                                enc1, enc2, driveTraj1PID, driveTraj2PID); 
 
double t, t_old, deltaT, print_time, t0 = 0;  // declare some time variables

// Initialize data storage variables
float distCm; // Distance from distance sensor in cm

// Active order storage variable
ReceiveOrder::BlockColor activeOrder[ReceiveOrder::MAX_BLOCKS];
uint8_t activeOrderCount = 0;

// Initialize state variables
int state = 0;

float straightDistance;
float rotateAngle;
float curveAngle;
float curveRadius;
float trajectoryTime;

char commandBuffer[64];
uint8_t commandIndex = 0;
bool receivingCommand = false;

bool trajectoryStarted = false;

void applyCommand(char *command) {
  // The "\r" is included because println() sends \r\n.
  char *stateText = strtok(command, " \r\t");
  char *p1Text    = strtok(NULL, " \r\t");
  char *p2Text    = strtok(NULL, " \r\t");
  char *p3Text    = strtok(NULL, " \r\t");
  char *extraText = strtok(NULL, " \r\t");

  if (stateText == NULL) {
    Serial.println(F("Invalid command."));
    return;
  }

  int requestedState = atoi(stateText);
  bool valid = false;

  switch (requestedState) {
    case 0:  // Command: 0
      valid = (p1Text == NULL);
      break;

    case 1:  // Command: 1 distance_cm time_s
      valid = (p1Text != NULL && p2Text != NULL &&
               p3Text == NULL && extraText == NULL);

      if (valid) {
        straightDistance = atof(p1Text);
        trajectoryTime = atof(p2Text);
      }
      break;

    case 2:  // Command: 2 angle_deg time_s
      valid = (p1Text != NULL && p2Text != NULL &&
               p3Text == NULL && extraText == NULL);

      if (valid) {
        rotateAngle = atof(p1Text) * PI / 180.0f;
        trajectoryTime = atof(p2Text);
      }
      break;

    case 3:  // Command: 3 angle_deg radius_cm time_s
      valid = (p1Text != NULL && p2Text != NULL &&
               p3Text != NULL && extraText == NULL);

      if (valid) {
        curveAngle = atof(p1Text) * PI / 180.0f;
        curveRadius = atof(p2Text);
        trajectoryTime = atof(p3Text);
      }
      break;
  }

  if (!valid) {
    Serial.println(F("Invalid command."));
    return;
  }

  drive.stop();
  driveTrajectory.reset();
  trajectoryStarted = false;
  state = requestedState;

  Serial.print(F("State set to: "));
  Serial.println(state);
}

void receiveUnoCommand() {
  while (Serial2.available() > 0) {
    int incoming = Serial2.read();
    Serial.print(incoming);
    Serial.print(' ');

    if (incoming == 255) {
      receivingCommand = true;
      commandIndex = 0;
    }
    else if (receivingCommand && incoming == '\n') {
      commandBuffer[commandIndex] = '\0';
      receivingCommand = false;
      applyCommand(commandBuffer);
    }
    else if (receivingCommand && commandIndex < sizeof(commandBuffer) - 1) {
      commandBuffer[commandIndex++] = (char)incoming;
    }
  }
}

void setup() {
  Serial.begin(57600);      // for Serial Monitor messages
  Serial2.begin(115200);    // communication with the XBee / Uno. Make sure baud rates match

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
  
  receiveUnoCommand();

  // Set state directly from serial monitor. For debugging purposes
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

    case 1:{

      if (!trajectoryStarted) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(straightDistance, trajectoryTime); //  xf cm in t seconds
        trajectoryStarted = true;

        Serial.print(F("Starting straight: distance = "));
        Serial.print(straightDistance);
        Serial.print(F(" cm, time = "));
        Serial.println(trajectoryTime);
      } 
      
      driveTrajectory.update();

      if (trajectoryStarted &&
        driveTrajectory.state == DriveTrajectory::IDLE) {

        trajectoryStarted = false;  // Ready for a future trajectory
        state = 0;                  // Advance to next robot state
      }
    break;}

    case 2:{

      if (!trajectoryStarted) {
        driveTrajectory.reset();
        driveTrajectory.startRotate(rotateAngle, trajectoryTime); // phi rad in t seconds
        trajectoryStarted = true;
      }  
      
      driveTrajectory.update();
      
      if (trajectoryStarted &&
        driveTrajectory.state == DriveTrajectory::IDLE) {

        trajectoryStarted = false;  // Ready for a future trajectory
        state = 0;                  // Advance to next robot state
      }
    break;}

    case 3:{

      if (!trajectoryStarted) {
        driveTrajectory.reset();
        // Drive for phi radians in circle of radius r in t seconds
        driveTrajectory.startCurve(curveAngle, curveRadius, trajectoryTime); 
        trajectoryStarted = true;
      }  
      
      driveTrajectory.update();

      if (trajectoryStarted &&
        driveTrajectory.state == DriveTrajectory::IDLE) {

        trajectoryStarted = false;  // Ready for a future trajectory
        state = 0;                  // Advance to next robot state
      }
    break;}

  }
}
