const int distSensor = A4;
int val = 0;
float voltage = 0;
float distcm = 0;
float filteredVal = 0;

const float alpha = .2;

double t, t0, print_time=0; // declare some time variables

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  t0 = micros()/1000000.;
}

void loop() {
  t = micros()/1000000.-t0;

  
  val = analogRead(distSensor);
  filteredVal = alpha*val + (1-alpha) * filteredVal;
  voltage = filteredVal * (5.0 / 1023.0);

  distcm = exp((log(voltage) - 2.851f) / -0.8776f);

  
  // PRINT STATEMENTS
  // non-blocking way to delay printing  
  if ((t-print_time)>0.25) { 
    Serial.println(distcm);
    print_time=t;
  }
}
