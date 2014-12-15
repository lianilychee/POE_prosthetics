// by Team Redacted for POE Project ICF Prosthetics

/************LIBRARIES*****************/
#include <Servo.h> 

/************VARIABLES*****************/
//Variables for control 
Servo cuffServo;  // create servo object to control a servo 
Servo gripServo;

//Variables for raw input 
int fingerpin1 = 1; //analog pin used for a single finger sensor
int fingerpin2 = 2; 
int fingerpin3 = 3;
int grip; //This is for the pressure sensors
int grip1, grip2, grip3;
int potpin = 0;  // analog pin used to connect the potentiometer
int bend;    // This is for the elbow potentiometer 

//Variables for smoothing
const int numReadings = 20;

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

/************SETUP*****************/ 
void setup() 
{ 
  Serial.begin(9600); // for debugging in terminal
  cuffServo.attach(9);  // attaches the servo on pin 9 to the servo object 
  gripServo.attach(10);
  
  for (int thisReading=0; thisReading < numReadings; thisReading++){
    gripReadings[thisReading] = 0;
    bendReadings[thisReading] = 0;
  }
  
  relaxed = 1;
} 

/************MAIN LOOP*****************/ 
void loop() 
{ 
  grip1 = analogRead(fingerpin1);
  grip2 = analogRead(fingerpin2);
  grip3 = analogRead(fingerpin3);
  grip = max(grip1, grip2);
  grip = max(grip, grip3);
  bend = analogRead(potpin);  // reads the value of the potentiometer (value between 0 and 1023) 
  
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
  Serial.println(bendAverage);
  //Serial.println(gripAverage);
  
  //Scale the averages to have a sensical input for the servos
  grip = map(gripAverage, 0, 30, 0, 179);     // Grip bounds may need to be adjusted (this is where calibration tests will be useful!) 
  //cuffServo.write(grip);                  // sets the servo position according to the scaled value 
  
  bend = map(bendAverage, 0, 1023, 0, 359);     // Probably also needs to be adjusted, scale it to use it with the servo (value between 0 and 180) 
  //gripServo.write(bend);                  // sets the servo position according to the scaled value 
    
  if (relaxed) { relax(); Serial.println("relaxed"); }
  else if (lift) { arm_lift(); Serial.println("lift"); }
  else if (grabbing) { grab(); Serial.println("grab"); }
  else if (hold) { hold_stuff(); Serial.println("hold"); }
  else if (reelees) { reelees_now(); Serial.println("release"); }
} 
/****************RELAX LOOP*************/
void relax() {
  //If arm is by our side, relax.  If it moves, its a lift!
  if (bendAverage < 140) {               
    relaxed = 1;
    gripServo.write(70);                 //'Relaxed' position of somewhat closed
    delay(15);                           // waits for the servo to get there 
  }
  else {
    relaxed = 0;
    lift = 1;
    gripServo.write(0);
    delay(15);
  }
  
  cuffServo.write(0);
  delay(15);
}
/**************LIFT LOOP***************/
void arm_lift() {
  //If arm bent, then lifting forearm.  If we extend, then we're reaching
 if (bendAverage > 140) {
  lift = 1;
  gripServo.write(0);
 }
 else {
   lift = 0;
   grabbing = !grabbing;
   reach = 1;
 }
  cuffServo.write(0);
  delay(15);
}
/***********GRAB LOOP****************/
void grab() {
  //If we're not maxed out in force, or over our movement range and arm is extended, grip!
  if (gripAverage < 300 && bendAverage < 150 && looper < 5000) {
    grabbing = 1;
    reach = 0;
    cuffServo.write(grip);
    Serial.println("CUFF SERVO GO");
    gripServo.write(map(looper, 0, 5000, 0, 130));
    looper = looper + 10;
    
    delay(15);
  }
  else {
    gripServo.write(map(looper-1, 0, 5000, 0, 130));
    cuffServo.write(lastGrip);
    Serial.println("CUFF SERVO GO");
    hold = 1;
    grabbing = 0;
    //    delay(5000); //leave time for user to move to comfortable holding position
  }
}
/**************HOLD LOOP*************/
void hold_stuff() {
  //if extend, we're releasing
  if (bendAverage > 140) {
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
    gripServo.write(map(looper, 0, 5000, 0, 130));
//    gripServo.write(looper);
    cuffServo.write(lastGrip);
    lastGrip = grip;
    Serial.println("CUFF SERVO GO");
    looper = looper - 10;
    delay(15);
  }
  else {
    relaxed = 1;
    lastGrip = 0;
    hold = 0;
    reach = 0;
    lastGrip = 0;
    looper = 0;
    grabbing = 0;
    reelees = 0;
    cuffServo.write(lastGrip);
    Serial.println("CUFF SERVO GO");
    gripServo.write(looper);
    Serial.println("reset");
    delay(15);
  }
}
    
