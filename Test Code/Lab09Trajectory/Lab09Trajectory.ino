// Lab9Template.ino
// Prof. Stephen Mascaro
// 10/23/23

//  This sketch will be used to perform line localization and line following
#include <Encoder.h>
#include <DualTB9051FTGMotorShieldMod3230.h>

Encoder myEnc1(19, 18);  // create an encoder object, both interrupts
Encoder myEnc2(20, 21);


DualTB9051FTGMotorShieldMod3230 md;  // create a motor driver object

double t, t_old, deltaT, print_time, t0 = 0;  // declare some time variables

double Kp = 100;  //Proportional Gain for Trajectory Following

long counts1, counts2, counts3;  // encoder counts
int m1c = 100, m2c = 0;            //declare and initialize motor commands

double GearRatio = 70;  // gear ratio
int countsPerRev = 64;  // encoder counts per Rev
double rw = 4.2;          // wheel radius in cm
double D = 22.2;          // distance between wheels in cm
double xf = 50;         // final disance
double phif = 3.1415;   // final angle in rotation
double phid = 1.5707;   // final angle in turn
double R = 50;         // turn radius disance

double theta1, theta1_old = 0, omega1;     //position and velocity of wheel 1
double theta2, theta2_old = 0, omega2;     //position and velocity of wheel 2
double omega1f, omega2f = 0, alpha = 0.2;             // filtered velocity and filter weight
double theta1_des = 0, theta2_des = 0;     // desired position of wheels
double theta1_final, theta2_final;         // final desired position of wheels
double omega_des, omega1_des, omega2_des;  // desired velocity of wheels

const double positionTolerance = 0.1;  // rad
double maxT = 10; // max time for each maneuver

int state = 0;

void setup() {
  // initialize reflectance sensor and motor driver
  md.init();
  md.enableDrivers();
  md.flipM1(true);

  Serial.begin(9600);
  t_old = micros() / 1000000.;
  t0 = micros() / 1000000.;
}

void loop() {

  t = micros() / 1000000. - t0;
  deltaT = t - t_old;  // sample time

  counts1 = myEnc1.read();
  counts2 = myEnc2.read();


  // calculate your position and velocity here
  theta1 = 2*PI*counts1/(countsPerRev*GearRatio);
  theta2 = 2*PI*counts2/(countsPerRev*GearRatio);

  omega1 = (theta1-theta1_old)/deltaT;
  omega2 = (theta2-theta2_old)/deltaT;

  omega1f = alpha * omega1 + (1-alpha) * omega1f;
  omega2f = alpha * omega2 + (1-alpha) * omega2f;

  // add your trajectory design here
  switch(state){
    case 0:{
    break;}

    case 1:{
    break;}

    case 2:{
    break;}
  }

  // drive straight 50 cm
  // theta1_final = xf/rw; // final distance 50 cm
  // theta2_final = xf/rw; // final distance 50 cm
  // omega1_des = theta1_final/maxT; // rad/s
  // omega2_des = theta2_final/maxT; // rad/s 

  // turn in place 180 deg
  // theta1_final = -phif*D*0.5/rw; 
  // theta2_final = phif*D*0.5/rw;
  // omega1_des = theta1_final/maxT; // rad/s
  // omega2_des = theta2_final/maxT; // rad/s 

  // Drive in circle of r = 50 cm
  // theta1_final = phid*R/rw; 
  // theta2_final = phid*(R+D)/rw;
  // omega1_des = theta1_final/maxT; // rad/s
  // omega2_des = theta2_final/maxT; // rad/s 
  
  theta1_des = theta1_des + omega1_des*deltaT;
  theta2_des = theta2_des + omega2_des*deltaT;

  // add your control laws here
  m1c = Kp*(theta1_des-theta1);
  m2c = Kp*(theta2_des-theta2);
  m1c = constrain(m1c, -400, 400);
  m2c = constrain(m2c, -400, 400);

  if (abs(theta1_final - theta1) < positionTolerance &&
    abs(theta2_final - theta2) < positionTolerance) {
    m1c = 0;
    m2c = 0;
  }

  // Uncomment these four lines in section 4.4
  // V1m = constrain(V1m,-10,10);
  // V2m = constrain(V2m,-10,10);
  // m1c = 400*V1m/10;
  // m2c = 400*V2m/10;
  
  // send motor commands
  md.setM1Speed(m1c);
  md.setM2Speed(m2c);

  if ((t - print_time) > 0.25) {  // non-blocking way to delay printing
    // print any variables of interest inside this if statement
    Serial.print(theta1);
    Serial.print('\t');
    Serial.print(theta1_final);
    Serial.print('\t');
    Serial.print(m1c);
    Serial.print('\t');
    Serial.print(theta2);
    Serial.print('\t');
    Serial.print(theta2_final);
    Serial.print('\t');
    Serial.println(m2c);


    print_time = t;
  }

  t_old = t;
  theta1_old = theta1;
  theta2_old = theta2;

}
