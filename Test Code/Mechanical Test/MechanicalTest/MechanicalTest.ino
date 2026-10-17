#include "DualTB9051FTGMotorShieldMod3230.h"
#include "ServoControl.h"

// class: name: pin assignment --> objects
// servo objects
ServoControl armFallServo(44);
ServoControl pickButtonServo(46);
ServoControl paddleServo(45);
ServoControl shipButtonServo(13);
ServoControl leverServo(11);

/*
int state = 0;
unsigned long lastChangeTime = 0;
const unsigned long interval = 1000;  // milliseconds
*/

// Uses the shield's default M1/M2 pin mapping:
// M1: EN=2, DIR=7, PWM=9; M2: EN=4, DIR=8, PWM=10.
// M1DIAG and M2DIAG do not need to be connected for this test.
DualTB9051FTGMotorShieldMod3230 md;

// servo state variables
bool armFallPosition = 0;
bool pickButtonPosition = 0;
bool paddlePosition = 0;
bool shipButtonPosition = 0;
bool leverPosition = 0;

// drive motor variables
enum Motion {DRIVESTOPPED, FORWARD, BACKWARD, CW, CCW};

Motion motion = DRIVESTOPPED;
unsigned long motionStartTime = 0;
const unsigned long driveDuration = 1000;

const int DRIVESPEED = 100;


// RnP motor
const int IN1 = 5;    // PWM pin -> L298N D1 / IN1
const int IN2 = 3;   // PWM pin -> L298N D2 / IN2

enum rnpMOTION {RNPSTOPPED, EXTEND, RETRACT};
rnpMOTION rnpMotion = RNPSTOPPED;
unsigned long rnpMotionStartTime = 0;
const unsigned long rnpDriveDuration = 1000;

void setMotorSpeed(int speedValue) {
  // speedValue range: -255 to +255
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    analogWrite(IN1, speedValue);
    analogWrite(IN2, 0);
  }
  else if (speedValue < 0) {
    analogWrite(IN1, 0);
    analogWrite(IN2, -speedValue);
  }
  else {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);  // brake
  }
}

// encoder pins


// setup function
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  // Match the Uno/XBee link baud rate. 9600 is reliable with the Uno's
  // SoftwareSerial implementation.
  Serial1.begin(115200);

  md.init();
  md.enableDrivers();
  md.flipM1(true);

  md.setM1Speed(0);
  md.setM2Speed(0);

  // initialize servo objects and set the angle 
  armFallServo.begin();
  armFallServo.setAngle(0);

  pickButtonServo.begin();
  pickButtonServo.setAngle(90);
  
  paddleServo.begin();
  paddleServo.setAngle(0);

  shipButtonServo.begin();
  shipButtonServo.setAngle(90);

  leverServo.begin();
  leverServo.setAngle(90);

// startup message
  Serial.println(F("Mega robot ready"));
  Serial.println(F("Waiting for XBee commands"));

}

// Initialize command string
char command = "a";

void loop() {
// check for wireless command from uno

// check if uno sent something
  // A message contains two bytes: 255 followed by the command character.
  // Wait for both so Serial1.read() never returns -1 for the command.
  if (Serial1.available() >= 2 && Serial1.read() == 255) {
    command = Serial1.read();

    Serial.print(F("Mega received: "));
    Serial.println(command);

    // determine what robot should do
    switch (command) {

      case 'F':
        // Drive forward for some time
        md.setM1Speed(DRIVESPEED);
        md.setM2Speed(DRIVESPEED);
        motion = FORWARD;
        motionStartTime = millis();   // restarts the timer
        break;

      case 'B':
        // Drive backward for some time
        md.setM1Speed(-DRIVESPEED);
        md.setM2Speed(-DRIVESPEED);
        motion = BACKWARD;
        motionStartTime = millis();   // restarts the timer
        break;

      case '>':
        // rotate CW for some time
        md.setM1Speed(DRIVESPEED);
        md.setM2Speed(-DRIVESPEED);
        motion = CW;
        motionStartTime = millis();   // restarts the timer
        break;

      case '<':
        // rotate CCW for some time
        md.setM1Speed(-DRIVESPEED);
        md.setM2Speed(DRIVESPEED);
        motion = CCW;
        motionStartTime = millis();   // restarts the timer
        break;

      case 'E':
        // Extend RnP for some time
        setMotorSpeed(200);
        rnpMotion = EXTEND;
        rnpMotionStartTime = millis();
        break;

      case 'R':
        // Retract RnP for some time
        setMotorSpeed(-200);
        rnpMotion = RETRACT;
        rnpMotionStartTime = millis();
        break;  

      case 'A':
        // Move armFall servo
        switch (armFallPosition){
          case 0:
            armFallServo.setAngle(180);
            armFallPosition = 1;
            break;
          case 1:
            armFallServo.setAngle(0);
          armFallPosition = 0;
            break;
        }
        break;

      case 'P':
        // Pick button 
        switch (pickButtonPosition){
          case 0:
            pickButtonServo.setAngle(0);
            pickButtonPosition = 1;
            break;
          case 1:
            pickButtonServo.setAngle(90);
            pickButtonPosition = 0;
            break;
        }
        break;

      case 'D':
        // Paddle servo
        switch (paddlePosition){
          case 0:
            paddleServo.setAngle(90);
            paddlePosition = 1;
            break;
          case 1:
            paddleServo.setAngle(0);
            paddlePosition = 0;
            break;
        }
        break;

      case 'H':
        // Ship button servo
        switch (shipButtonPosition){
          case 0:
            shipButtonServo.setAngle(0);
            shipButtonPosition = 1;
            break;
          case 1:
            shipButtonServo.setAngle(90);
            shipButtonPosition = 0;
            break;
        }
        break;

      case 'L':
        // Lever servo
        switch (leverPosition){
          case 0:
            leverServo.setAngle(45);
            leverPosition = 1;
            break;
          case 1:
            leverServo.setAngle(135);
            leverPosition = 0;
            break;
        }
        break;
    }
  }

  if (motion != DRIVESTOPPED && millis() - motionStartTime >= driveDuration) {
    // stop drive motors after some time
    md.setM1Speed(0);
    md.setM2Speed(0);
    motion = DRIVESTOPPED;
  }

  if (rnpMotion != RNPSTOPPED && millis() - rnpMotionStartTime >= rnpDriveDuration) {
    // stop rnp drive motors after some time
    setMotorSpeed(0);
    rnpMotion = RNPSTOPPED;
  }
}
