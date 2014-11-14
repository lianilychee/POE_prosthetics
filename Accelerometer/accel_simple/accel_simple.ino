#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Servo.h"

MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;

//Servo myservo;

int yVal;
int prevVal;

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

    Serial.print("ay unfiltered: ");
    Serial.println(ay);
    delay(100);

    yVal = map(ay, -17000, 17000, 0, 179);
    if (yVal != prevVal)
    {
//        myservo.write(val);
        Serial.print("ay FILTERED: ");
        Serial.println(yVal);
        prevVal = yVal;
    }

    delay(100);
}
