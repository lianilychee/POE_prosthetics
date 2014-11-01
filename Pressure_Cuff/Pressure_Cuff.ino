// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 
// by Victoria Preston for POE Project ICF Prosthetics

/************LIBRARIES*****************/
#include <Servo.h> 


/************VARIABLES*****************/
//Variables for control 
Servo myservo;  // create servo object to control a servo 

//Variables for raw input 
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 

//Vairables for smoothing
const int numReadings = 10;

int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

/************SETUP*****************/ 
void setup() 
{ 
  Serial.begin(9600); // for debugging in terminal
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  
  for (int thisReading=0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
} 

/************MAIN LOOP*****************/ 
void loop() 
{ 
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  
  total = total-readings[index];
  readings[index] = val;
  total = total + readings[index];
  index = index + 1;
  
  if (index >= numReadings)
    index = 0;
  
  average = total/numReadings;
  Serial.println(average);
  
  val = map(average, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
} 
