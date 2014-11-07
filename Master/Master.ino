/* Code for ICFProsthetics 
Written by: Ellie Funkhouser, Liani Lye, Victoria Preston, and Celine Ta
Last Update: November 1, 2014
*/

/***********LIBRARIES****************/
#include <Servo.h>

/***********VARIABLES****************/
//Assign Servos
Servo cuffservo;
Servo gripservo;

//State variables
// 1 = rest, 2 = lift, 3 = reach, 4 = grab, 5 = hold, 6 = release
int state = 1; 

//Assign pins
int gripSensor = A5; 

//State Parameters
int relaxedAngle = 90; //0-180 extremes, 90 = no movement
int gripSpeed = 1;

//Variables for raw input
int joint_pin = 0;
int angle;
int finger_a = 1;
int a;
int finger_b = 2;
int b;
int finger_c = 3;
int c;
//TODO Accelerometer Data
//TODO Dtate Booleans

//Variables for smoothing
const int numReadings = 10;
int index = 0;

int joint_readings[numReadings];
int a_readings[numReadings];
int b_readings[numReadings];
int c_readings[numReadings];

int joint_total = 0;
int a_total = 0;
int b_total = 0;
int c_total = 0;

int joint_avg = 0;
int a_avg = 0;
int b_avg = 0;
int c_avg = 0;



/***********SETUP****************/
void setup()
{
  Serial.begin(9600); //for debugging
  cuffservo.attach(9, 1000, 2000); //assume 0-180 servo, 1000/2000 are the mix/max PWM
  gripservo.attach(10, 1000, 2000); //assume 0-180 servo
  
  for (int thisReading=0; thisReading < numReadings; thisReading++)
    joint_readings[thisReading] = 0;
    a_readings[thisReading] = 0;
    b_readings[thisReading] = 0;
    c_readings[thisReading] = 0;
      
  //TODO Relax the Actuators?
  //TODO Calibrate the Accel?
}

/***********MAIN LOOP****************/
void loop()
{
  //TODO Add Controller If-Statement
  /* Read in State
  Determine Next State
    if(relaxed) -> lift
    if (lift) -> reach
    if (reach, no hold) -> grab  
    if (reach, held) -> release
    if(grab) -> hold
    if (not extended, held) -> lift (note: no actuation)
    if (extended, held) -> release
  Call next state*/
  gripservo::refresh //Do we need this? 
}

/***********FINGER SUBLOOP***********/
//If readings > some threshold (50?) for 15+ readings, allow as real feedback to cinching servo */
int fingerFeedback(signal){
	//check if not noise

	//if not noise, send to smoothing
}

/*Actuation commands:
relax() -> gripper (semi)


lift()


reach() -> gripper (open)
	set finger servo: increment position to full open

grab()
set finger servo: increment position // see Actuator sketch for example

// now start letting the person know that we’ve grabbed an object
while pressure sensor reads some value
	increment cuff servo
	// got to do mapping of pressure sensor to cuff servo position

// If we move the lower arm or if the servo is over torqued, exit
if (angle shift || finger_pressure > threshold)
exit function

	
	hold()
		finger servo position = previous finger servo position
		
	release()
		set finger servo: increment to open*/

void relax(){
	cuffservo.write(relaxedAngle);
	gripservo.write(relaxedAngle);
	//TODO: Check if this will move servos simultaneously
	stage = 1;
}

void lift(){
	stage = 2;
}

void reach(){
	stage = 3;
	for(int p= relaxedAngle; p<180; p++){ //may be p > 0 depending
		gripservo.write(p*gripSpeed);
	}
}

void grab(){
	stage = 4;
	//TODO: read from accelerometer/ pass in data to variable accel, replace someThreshold with number
	if(accelNow- accelPrev > some_Threshold){
		stopSig = true;
	}
	while(!stopSig || currentPos > safetyThreshold){ //may be < depending
		currentPos = gripservo.read();
		gripservo.write(currentPos + 1) //may be -1 depending
		gripRead = analogRead(gripSensor);
		cuffservo.write(map(gripRead, 0, 500, 90, 180)) // scale mapping parameters as necessary
	}
	gripStop = gripservo.read();

}

void hold(){
	//TODO: Check for trigger to release
	gripservo.write(gripStop);
	stage = 5;
}

void release(){
	for(int r= gripStop; r>90; r--){ //may be p > 0 depending
		gripservo.write(r*gripSpeed);
	}
}