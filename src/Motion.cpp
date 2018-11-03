#include <Arduino.h>
#include "Motion.h"
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);

void Motion::initialize()
{
    Serial.print("\nMotion initializing ... ");

    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

    Serial.println("done");
}