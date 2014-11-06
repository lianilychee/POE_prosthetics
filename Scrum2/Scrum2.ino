// by Victoria Preston for POE Project ICF Prosthetics

/************LIBRARIES*****************/
#include <Servo.h> 


/************VARIABLES*****************/
//Variables for control 
Servo cuffServo;  // create servo object to control a servo 
Servo gripServo;

//Variables for raw input 
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
} 

/************MAIN LOOP*****************/ 
void loop() 
{ 
  grip = analogRead(fingerpin);
  bend = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  
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
  cuffServo.write(grip);                  // sets the servo position according to the scaled value 
  
  bend = map(bendAverage, 0, 1023, 0, 179);     // Probably also needs to be adjusted, scale it to use it with the servo (value between 0 and 180) 
  gripServo.write(bend);                  // sets the servo position according to the scaled value 
  
  delay(15);                           // waits for the servo to get there 
} 
