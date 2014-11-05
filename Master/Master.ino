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
  cuffservo.attach(9);
  gripservo.attach(10);
  
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
