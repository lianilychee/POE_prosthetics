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
