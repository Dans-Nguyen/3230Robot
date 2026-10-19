// Lab7Template.ino
// Prof. Stephen Mascaro
// 10/05/23

//  This sketch will be used to perform line localization and line following

// Library imports
#include <QTRSensors.h>
#include "DualTB9051FTGMotorShieldMod3230.h"

// Library objects
QTRSensors qtr; // create a reflectance sensor object
DualTB9051FTGMotorShieldMod3230 md; // create a motor driver object


// Global variables
const uint8_t SensorCount = 8;  // # of sensors in reflectance array
uint16_t sensorValues[SensorCount];  //reflectance sensor readings
double t, t0, print_time=0; // declare some time variables

double Kp=1; //Proportional Gain for Line Following
double base_speed=100; //Nominal speed of robot

int m1c=0, m2c=0;  //declare and initialize motor commands

// Remember to use the correct syntax for initializing an array.
// The array needs to know how large it is along with any initial values
// datatype array[size];
// datatype array2[3] = {0, 0, 0};

int sensor_bias[8] = {44,	44,	92,	92,	92,	92,	92,	92};
int sensor_values_unbiased[8] = {0, 0, 0, 0, 0, 0, 0, 0};
double num = 0;
double den = 0;
double d[8] = {0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6}; // mm 
double d_sum = 0;
double error = 0;

void setup() {
  // initialize reflectance sensor and motor driver
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){37, 36, 39, 38, 41, 40, 43, 42},SensorCount);
  // {42, 43, 41, 40, 39, 38, 37, 36}
  md.init(); 
  md.enableDrivers();
  md.flipM1(true);



  Serial.begin(9600);
    
  // Uncomment the following code when you get to the relevant section in lab
  // in order to input Kp and base_speed from Serial Monitor
  
  String inString = "";
  Serial.println("Input values for Kp and base_speed with a space between");
  while(Serial.available()==0);
  inString = Serial.readStringUntil(' ');
  Kp = inString.toFloat();
  inString = Serial.readStringUntil(' ');
  base_speed = inString.toFloat();
  
  t0 = micros()/1000000.;
  Serial.print("Kp = ");
  Serial.println(Kp);
  Serial.print("Base Speed = ");
  Serial.println(base_speed);


  t0 = micros()/1000000.; // initialize time
}




void loop() {
  d_sum = 0;
  num = 0;
  den = 0;
  t = micros()/1000000.-t0;
  qtr.read(sensorValues); // sensor values will be numbers between 0 and 2500
  
  ////////// LINE LOCALIZATION CODE
  // Remember to use for loops for assigning values to an array, you cannot
  // assign values to multiple array indicies at once
  // for (datatype var = value; var < breakpoint; var++)
  // The above structure:
  //      Initializes a temporary variable
  //      Sets a condition to loop while true
  //      Tells the temporary variable how to update after each loop

  for (int i = 0; i <= 7; i++){
    sensor_values_unbiased[i] = sensorValues[i] - sensor_bias[i];
  }

  for (int i = 0; i <= 7; i++){
    num = num + sensor_values_unbiased[i] * d[i];
    den = den + sensor_values_unbiased[i];
  }

  d_sum = num/den;
  error = d_sum - 2.8;

  if (error >= 2.5){
    error = 2.5;
  } else if (error <= -2.5){
    error = -2.5;
  }

  ////////// LINE FOLLOWING CODE
  m1c = base_speed + Kp * error; 
  m2c = base_speed - Kp * error;


  md.setM1Speed(m1c);
  md.setM2Speed(m2c);

  // PRINT STATEMENTS
  // non-blocking way to delay printing  
  if ((t-print_time)>0.25) { 
    
    // This for loop is used to print out variables that are arrays
    for (uint8_t i=0; i < SensorCount; i++)
    {
      Serial.print(sensor_values_unbiased[i]);
      
      Serial.print('\t');
    }
    Serial.print(d_sum);
    Serial.print('\t');
    Serial.print(error);
    // Print any non-array variables here
    Serial.println();
    print_time=t;
  }

}
