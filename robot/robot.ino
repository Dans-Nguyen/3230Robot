// Component classes
#include "components\SharpGP2Y0A21YK0F.h"
#include "components\QTRMD08RC.h"
#include "components\MD_TCS_3200.h"
#include "components\Hall_Effect.h"
#include "components\LTR_4206E_IR.h"
#include "components\LimSwitch.h"
#include "components\Motor20D.h"
#include "components\ServoControl.h"
#include <Encoder.h>

// Automation classes
#include "automation\PID.h"
#include "automation\DriveControl.h"
#include "automation\LineFollow.h"
#include "automation\DetectBlock.h"
#include "automation\ReceiveOrder.h"
#include "automation\DriveTrajectory.h"
#include "automation\RackTrajectory.h"
#include "automation\WallFollow.h"

// Initialize PID objects
PID lineFollowPID(300, 0, 10);  // PID object for line following
PID wallFollowPID(50, 0, 0);  // PID object for wall following
PID driveTraj1PID(600, 0, 10); // PID object for wheel 1 drive trajectory
PID driveTraj2PID(600, 0, 10); // PID object for wheel 2 drive trajectory
PID rackTrajPID(400, 0, 0); // PID object for rack trajectory

// Initialize encoder objects
Encoder enc1(19, 18); // Wheel 1 encoder
Encoder enc2(20, 21); // Wheel 2 encoder
Encoder enc3(3, 22);  // Rack motor encoder

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
LimSwitch limSwitch(53);
WallFollow wallFollow(distSensorR, drive, wallFollowPID);

// servo objects
ServoControl pickButtonServo(45, 90);
ServoControl paddleServo(44, 0);
ServoControl shipButtonServo(12, 90);
ServoControl leverServo(13, 30);

const int IN1 = 5;   // PWM pin -> L298N IN1. Give signal to extend
const int IN2 = 6;   // PWM pin -> L298N IN2. Give signal to retract
Motor20D motor20D(5, 6);


// Drive motor physical parameters
// GR = 50;            // gear ratio
// countsPerRev = 64;  // encoder counts per Rev
// rw = 4.2;           // wheel radius in cm
// D = 21.7;           // distance between wheels in cm

DriveTrajectory driveTrajectory(50.0f, 64, 4.2f, 21.7f, drive, 
                                enc1, enc2, driveTraj1PID, driveTraj2PID); 

// RnP motor physical parameters
// GR = 78;
// countsPerRev = 20;
// rw = 1.252;

RackTrajectory rackTrajectory(78.0f, 20, 1.252f, 
                              motor20D, enc3, limSwitch, rackTrajPID);

 
double t, t_old, deltaT, wallFollowStartT, t0 = 0;  // declare some time variables
bool wallFollowTiming = false;

// Initialize data storage variables
float distCm; // Distance from distance sensor in cm
float rackSpeed = 15.0f; // rack speed in cm/s

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
  limSwitch.begin();
  motor20D.begin();

  // initialize servo objects

  pickButtonServo.begin();  
  paddleServo.begin();
  shipButtonServo.begin();
  leverServo.begin();
  

  t0 = micros()/1000000.; // initialize time in seconds
}

void loop() {
  
  // Check for uno command
  if (Serial2.available() >= 2 && Serial2.read() == 255) {
    state = Serial2.read();

    Serial.print(F("Mega received: "));
    Serial.println(state);
  }

  // Set state directly from serial monitor. For debugging purposes
  if (Serial.available() > 0) {
    int requestedState = Serial.parseInt();

    if (requestedState != 999) {
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

    // RESETTING CASE
    case 0:{
      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.startHome(); // home
      }

      rackTrajectory.update();

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        state = 999;                  // Advance to next robot state
      }
    break;}

    // LEAVING STARTING AREA
    case 1:{ // line follow and extend rack
      lineFollow.start(200);

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(10.0, rackSpeed); // 10 cm at 5 cm/s
      }

      rackTrajectory.update();

      // Get distance data from front sensor
      distCm = distSensorF.getDistCm();


      if(distCm <= 30){
        // distance from wall and front sensor achieved, stop line following
        drive.stop();
        state = 2;
      }

    break;}

    case 2:{ // turn 90 degrees
      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        // Start command sets driveTrajecoty state to ACTIVE
        driveTrajectory.startRotate(PI/2.0f, 0.5); // 90 deg in 0.5 seconds
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 3;                  // Advance to next robot state
      }

    break;}

    case 3:{ // wall follow for some 
      // record time once 

      if (!wallFollowTiming) {
        wallFollowStartT = millis();
        wallFollowTiming = true;
      }
      
      //Serial.println(distSensorR.getDistCm());
      //Serial.println(wallFollowStartT);

      wallFollow.start(-200, 26);
      
      if (millis() - wallFollowStartT >= 8000UL){ // stop after 8 s
        wallFollow.stop();
        state = 4;
        wallFollowTiming = false;
      }
      

    break;}

    // PICKING STATION TRAJECTORY
    case 4:{

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(20.0, 1.0); // 20 cm in 1 seconds
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 999;                  // Advance to next robot state
      }
    break;}









    // 120 state for testing order reception
    case 120:{
      float frequency = IR.getFrequency();
      Serial.println(frequency);
      if (order.update()) {
        activeOrderCount = order.count();
        
        // A complete start → colors → end message was received.
        for (uint8_t i = 0; i < order.count(); i++) {
          activeOrder[i] = order.block(i); // store block color to activeOrder
          //Serial.println(activeOrder[i]);
          if(activeOrder[i] == 1){
            Serial.println("RED");
          } else if(activeOrder[i] == 2){
            Serial.println("YELLOW");
          } else if(activeOrder[i] == 3){
            Serial.println("BLUE");
          } else if(activeOrder[i] == 4){
            Serial.println("BROWN");
          } 


        }

        state = 0;
      }
      
    break;}

    // 121 state for testing block detection
    case 121:{
      detectBlock.update();
      char blockColor = detectBlock.getColor();
      bool isMagnet = detectBlock.isMagnet();
      Serial.print(blockColor);
      Serial.println(isMagnet);
      
    break;}

    // 122 state for testing trajectory
    case 122:{

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(50.0, 1.0); // 50 cm in 6 seconds
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 999;                  // Advance to next robot state
      }
    break;}

    // 123-125 state for testing rack trajectory
    case 123:{
      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(20.0, 15.0); // 10 cm at 5 cm/s
      }

      rackTrajectory.update();

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        state = 124;                  // Advance to next robot state
      }
    break;}

    case 124:{
      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(10.0, 15.0); // 10 cm at 5 cm/s
      }

      rackTrajectory.update();

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        state = 125;                  // Advance to next robot state
      }
    break;}

    case 125:{
      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(5.0, 15.0); // 10 cm at 5 cm/s
      }

      rackTrajectory.update();

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        state = 123;                  // Advance to next robot state
      }
    break;}
  
  }
}
