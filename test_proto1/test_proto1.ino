#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
const int ir_read = A0;
int state;
int counter=0;
int counter_prev;
int sensorVal;
int desired_pos_slice;
int error;
int P;
int I;
int control_sig;
int desired_pos_deg = 90; // desired position in degrees; SET THIS FOR TESTING
int kp = 1;
int ki = 2;
int slice_angle = 15;
int error_threshold = 20;
int dir=1;
int white_thresh = 800;
int black_thresh = 700;
int flag = 0;

float in;
float sineout[101];

void setup() {
  Serial.begin(9600);  // set up Serial library at 9600 bps
  AFMS.begin();        // create with the default frequency 1.6KHz
  myMotor->setSpeed(20);
  
  
}

void loop() {
  //uint8_t i;
  int index = 0;
  //  myMotor->run(FORWARD);

//  int i = 0;
//  for (in = 0; in < 6.283; in = in + 0.0628)
//  {
//    sineout[i] = sin(in) * 180;
//    i++;
//  }
//  // // Convert desired_pos to ticks
//  desired_pos_slice = (desired_pos_deg / slice_angle) + 1; // denominator is angle of each slice; SET THIS FOR NEW PINWHEEL
//  desired_pos_deg = sineout[index];
  
  
  detect_slice();
  adjust_speed();

  
//  index++;

}

void adjust_speed(){
  // error = how much distance is left
  //sin(millis())*180;
  error = desired_pos_deg - (counter * slice_angle);
  Serial.print("Error: "); Serial.print(error);
  
//sin(millis()*.1)*180
  P = error * kp;
  I += ki * error;
  control_sig = P + I;
  Serial.print(", Control: "); Serial.print(control_sig);

  if (control_sig > 100){ 
    control_sig = 100; 
  }
  if (error >= slice_angle){ 
    myMotor->run(FORWARD);
    dir = 1;
    myMotor->setSpeed(control_sig); 
  }
  if (error < 0){ 
    myMotor->run(BACKWARD); 
    dir = -1;
    myMotor->setSpeed(control_sig); 
    flag = 1;
  }
  
//  if(flag == 1){
//    //myMotor->setSpeed(0);
//    desired_pos_deg = 0;
//    flag = 0;
//  }
  if (error > -slice_angle  && error < slice_angle ) {
//    myMotor->setSpeed(0);
    desired_pos_deg = 180;
  }

}

void sinusoid(){
  float in;
  float sineout[101];
  int i = 0;
  for (in = 0; in < 6.283; in = in + 0.0628)
  {
    sineout[i] = sin(in) * 180;
    i++;
    delay(1);
  }
  //return sineout;
}

void detect_slice(){
  sensorVal = analogRead(ir_read);
  // if sensing black slice when prev state is white,
  if (sensorVal > white_thresh && state == 1 && dir == 1) { state = 0; counter++; }
  // if sensing white slice when prev state is black,
  if (sensorVal < black_thresh && state == 0 && dir == 1) { state = 1; counter++; }
  if (sensorVal > white_thresh && state == 1 && dir ==-1) { state = 0; counter--; }
  if (sensorVal < black_thresh && state == 0 && dir==-1) { state = 1; counter--; }

 // Serial.print("state: "); Serial.println(state);
//  Serial.print("ir_read: "); Serial.println(sensorVal);
  Serial.print(", Commanded: "); Serial.print(desired_pos_deg);
  
  Serial.print(", Measured: "); Serial.println(counter*slice_angle);
  
  delay(50);
}

