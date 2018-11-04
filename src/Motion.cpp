#include <Arduino.h>
#include "Motion.h"
#include <MPU6050_tockn.h>

// https://github.com/tockn/MPU6050_tockn 

float currX = 0;
float currY = 0;
float currZ = 0;

float prevX = 0;
float prevY = 0;
float prevZ = 0;

int percentX = 0;
int percentY = 0;
int percentZ = 0;

// Library object
MPU6050 mpu6050(Wire);

void Motion::initialize()
{
    Serial.print("\nMotion initializing ... ");

    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

    Serial.println("done");
}

void Motion::update()
{
    mpu6050.update();

    currX = mpu6050.getAngleX();
    currY = mpu6050.getAngleY();
    currZ = mpu6050.getAngleZ();
    
    calculatePercentage();

    prevX = currX;
    prevY = currY;
    prevZ = currZ;
}

int Motion::getMovement() const
{
    int movement = percentX + percentY + percentZ;
    if (movement > 1000) {
        movement = 1000;
    }
    return movement;
}

// Private

void Motion::calculatePercentage()
{
    // Calculate X percentage
    auto increase = int(currX) - int(prevX);
    percentX = (increase / currX) * 100;
    if (percentX < 0) {
        auto decrease = int(prevX) - int(currX);
        percentX = (decrease / prevX) * 100;
    }

    // Calculate Y percentage
    increase = int(currY) - int(prevY);
    percentY = (increase / currY) * 100;
    if (percentY < 0) {
        auto decrease = int(prevY) - int(currY);
        percentY = (decrease / prevY) * 100;
    }

    // Calculate Z percentage
    increase = int(currZ) - int(prevZ);
    percentZ = (increase / currZ) * 100;
    if (percentZ < 0) {
        auto decrease = int(prevZ) - int(currZ);
        percentZ = (decrease / prevZ) * 100;
    }

    /*Serial.print("X%: ");
    Serial.print(percentX);
    Serial.print("\tY%: ");
    Serial.print(percentY);
    Serial.print("\tZ%: ");
    Serial.println(percentZ);*/
}