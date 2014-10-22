#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

int pace;
int i;
unsigned long testing;
float pot_value;
int pot_pin = 0;
int buttonpin= 8; 
int past_pot = 0;
int velocity = 0;

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  
  pinMode(pot_pin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;   
  
}

void loop() {
  myMotor->setSpeed(velocity);
  past_pot = pot_value;
  
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    myMotor->run(RELEASE);
  }
  else
  {
          // subtract the last reading:
        total= total - readings[index];         
        // read from the sensor:  
        readings[index] = analogRead(pot_pin); 
        // add the reading to the total:
        total= total + readings[index];       
        // advance to the next position in the array:  
        index = index + 1;                    
      
        // if we're at the end of the array...
        if (index >= numReadings)              
          // ...wrap around to the beginning: 
          index = 0;                           
      
        // calculate the average:
        pot_value = total / numReadings;  
      
        
        if (pot_value > past_pot) {
          myMotor->run(FORWARD);
          velocity = pot_value*(255.0/1023.0);
        }
        else if (pot_value < past_pot) {
          myMotor->run(BACKWARD);
          velocity = abs(pot_value*(255.0/1023.0)-255.0);
        }
        else {
          myMotor->run(RELEASE);
        }
  }
 
  delay(20);
  
}
