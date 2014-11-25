// by Celine Ta and Victoria Preston for POE Project ICF Prosthetics

/************LIBRARIES*****************/
#include <Servo.h> 
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
/************VARIABLES*****************/
//Variables for control 
Servo cuffServo;  // create servo object to control a servo 
Servo gripServo;

//Variables for raw input 
MPU6050 mpu; // create accelerometer object for raw input
int16_t ax, ay, az; // raw parameters for accelerometer
int16_t gx, gy, gz; // raw parameters for gyroscope

//TODO: Re-calibrate for grabbing and when mounted on arm. 
int accelThresh= 9; // 4, threshold to determine sensitivity of moving vs. not moving for mapped accelerometer values

int fingerpin = 0; //analog pin used for a single finger sensor
int grip; //This is for the pressure sensors
int potpin = 1;  // analog pin used to connect the potentiometer
int bend;    // This is for the elbow potentiometer 

//Vairables for smoothing
const int numReadings = 10;

int gripReadings[numReadings];
int gripIndex = 0;
int gripTotal = 0;
int gripAverage = 0;

int bendReadings[numReadings];
int bendIndex = 0;
int bendTotal = 0;
int bendAverage = 0;

//Variables indicating state
boolean relaxed = 0;
boolean lift = 0;
boolean reach = 0;
boolean grabbing = 0;
boolean hold = 0;
boolean reelees = 0;

//Tracking variables
int looper = 0;
int lastGrip;
int xVal, yVal, zVal; // mapped accelerometer values from -17000-17000 to 
int prevX, prevY, prevZ; //tracking "memory" variable for mapped accelerometer values


/************SETUP*****************/ 
void setup() 
{ 
  Serial.begin(9600); // for debugging in terminal
  cuffServo.attach(9);  // attaches the servo on pin 9 to the servo object 
  gripServo.attach(10);
  
  Wire.begin();
  mpu.initialize(); // initialize accelerometer
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
  
  for (int thisReading=0; thisReading < numReadings; thisReading++){
    gripReadings[thisReading] = 0;
    bendReadings[thisReading] = 0;
  }
  
  relaxed = 1;
} 

/************MAIN LOOP*****************/ 
void loop() 
{ 
  //get raw data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // reads raw accelerometer/ gyroscope values
  grip = analogRead(fingerpin);
  bend = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  
  //TODO: Filter/ Smooth accelerometer values before mapping
    xVal = map(ax, -17000, 17000, -100, 100);
    yVal = map(ay, -17000, 17000, -100, 100);
    zVal = map(az, -17000, 17000, -100, 100);
    
  //Find the rolling average for the sensors (smoothing)
  bendTotal = bendTotal-bendReadings[bendIndex];
  bendReadings[bendIndex] = bend;
  bendTotal = bendTotal + bendReadings[bendIndex];
  bendIndex = bendIndex + 1;
  
  gripTotal = gripTotal-gripReadings[gripIndex];
  gripReadings[gripIndex] = grip;
  gripTotal = gripTotal + gripReadings[gripIndex];
  gripIndex = gripIndex + 1;
  
  if (bendIndex >= numReadings)
    bendIndex = 0;
    
  if (gripIndex >= numReadings)
    gripIndex = 0;
  
  bendAverage = bendTotal/numReadings;
  gripAverage = gripTotal/numReadings;
  //Serial.println(bendAverage);
  //Serial.println(gripAverage);
  
  //Scale the averages to have a sensical input for the servos
  grip = map(gripAverage, 0, 300, 0, 179);     // Grip bounds may need to be adjusted (this is where calibration tests will be useful!) 
  //cuffServo.write(grip);                  // sets the servo position according to the scaled value 
  
  bend = map(bendAverage, 0, 1023, 0, 179);     // Probably also needs to be adjusted, scale it to use it with the servo (value between 0 and 180) 
  //gripServo.write(bend);                  // sets the servo position according to the scaled value 
    
  if (relaxed) {
    relax(); 
    Serial.println("relaxed");
  }
  else if (lift) {
    arm_lift();
    Serial.println("lift");
  }
  else if (grabbing) {
    grab();
    Serial.println("grab");
  }
  else if (hold) {
    hold_stuff();
    Serial.println("hold");
  }
  else if (reelees) {
    reelees_now();
    Serial.println("release");
  }
  
  prevX = xVal;
  prevY = yVal;
  prevZ = zVal;
} 
/****************RELAX LOOP*************/
void relax() {
  //If arm is by our side (accel reads "down"), relax.  If it moves, its a lift!
  if (bendAverage > 300 && xVal > -75) {  
    relaxed = 0;
    lift = 1;
    gripServo.write(0);
    delay(15);                          // waits for the servo to get there 
  }
  else {
    Serial.println("Accel- arm down");             
    relaxed = 1;
    gripServo.write(90);                 //'Relaxed' position of somewhat closed
    delay(15); 
  }
  cuffServo.write(0);
  Serial.println("INCREMENTING CUFF SERVO");
  delay(15);
}
/**************LIFT LOOP***************/
void arm_lift() {
  //If arm bent, then lifting forearm.  If we extend, then we're reaching
 if (bendAverage > 300 && yVal> 80) {
  Serial.print("Accel- arm lifted");
  Serial.println(ay);
  lift = 1;
  gripServo.write(0);
 }
 else {
   if (yVal < 80) {
     lift = 0;
     relaxed = 1;
   }
   else {     
     lift = 0;
     grabbing = !grabbing;
     reach = 1;
   }
 }
  cuffServo.write(0);
  delay(15);
}
/***********GRAB LOOP****************/
void grab() {
  //If we're not maxed out in force, or over our movement range and arm is extended, grip!
  if (gripAverage < 300 && bendAverage < 400 && looper < 5000 && abs(xVal-prevX)<=accelThresh) {
    grabbing = 1;
    reach = 0;
    cuffServo.write(grip);
    Serial.println("INCREMENTING CUFF SERVO");
    gripServo.write(map(looper, 0, 5000, 0, 179));
    looper = looper + 1;
    lastGrip = grip;
    delay(15);
  }
  else {
    if(abs(xVal-prevX)>=accelThresh){
      Serial.println("Accel triggered-- hold");
    }
    gripServo.write(map(looper, 0, 5000, 0, 179));
    cuffServo.write(lastGrip);
    Serial.println("INCREMENTING CUFF SERVO");
    hold = 1;
    grabbing = 0;
    delay(5000); //leave time for user to move to comfortable holding position
  }
}
/**************HOLD LOOP*************/
void hold_stuff() {
  //if extend, we're releasing
  if (bendAverage > 300) {
    reelees = 0;
    hold = 1;
  }
  else {
    reelees = 1;
    hold = 0;
  }
}
/**************REELEES LOOP*********/
void reelees_now() {
  //when releasing, release down to 0.  Then reset.
  if (looper > 0) {
    gripServo.write(map(looper, 0, 5000, 0, 179));
    cuffServo.write(lastGrip);
    Serial.println("INCREMENTING CUFF SERVO");
    lastGrip = grip;
    looper = looper - 1;
    delay(15);
  }
  else {
    relaxed = 1;
    hold = 0;
    reach = 0;
    lastGrip = 0;
    looper = 0;
    grabbing = 0;
    reelees = 0;
    cuffServo.write(lastGrip);
    Serial.println("INCREMENTING CUFF SERVO");
    gripServo.write(looper);
    Serial.println("reset");
    delay(15);
  }
}
    
