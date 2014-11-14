#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Servo.h"

MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;

//Servo myservo;

int xVal, yVal, zVal;
int prevX, prevY, prevZ;

int accelThresh= 4;

void setup() 
{
    Wire.begin();
    Serial.begin(9600);

    Serial.println("Initialize MPU");
    mpu.initialize();
    Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
//    myservo.attach(9);
}

void loop() 
{
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    xVal = map(ax, -17000, 17000, 0, 179);
    yVal = map(ay, -17000, 17000, 0, 179);
    zVal = map(az, -17000, 17000, 0, 179);
    
    if (abs(xVal-prevX)>=accelThresh || abs(yVal-prevY)>=accelThresh || abs(zVal-prevZ)>=accelThresh)
    {
//        myservo.write(val);
        Serial.print("Moving - "); 
        Serial.print(xVal);
        Serial.print(",");
        Serial.print(yVal);
        Serial.print(",");
        Serial.println(zVal);
        prevX = xVal;
        prevY = yVal;
        prevZ = zVal;
    }else{
        Serial.println("Still"); 
    }

    delay(100);
}
