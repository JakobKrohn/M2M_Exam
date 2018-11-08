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

int currentMovement = 0;

std::vector<int> movements;

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

int Motion::getAverageMovement() const
{
    int average = 0;
    for (auto const& reading: movements) {
        average += reading;
    }

    //average = average / movements.size();
    if (average > 100) {
        average = 100;
    }
    
    movements.clear();

    return average;
}

int Motion::getCurrentMovement() const
{
    return currentMovement;
}

String Motion::getMovementString() const
{
    if (currentMovement > 90) {
        return "-----";
    } else if (currentMovement > 50) {
        return "----";
    } else if (currentMovement > 10) {
        return "---";
    } else if (currentMovement > 1) {
        return "--";
    } else {
        return "-";
    }
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

    // Calculate current movement
    currentMovement = percentX + percentY + percentZ;
    if (currentMovement > 100) {
        currentMovement = 100;
    }

    // Add this reading to vector
    movements.push_back(currentMovement);

    /*Serial.print("X%: ");
    Serial.print(percentX);
    Serial.print("\tY%: ");
    Serial.print(percentY);
    Serial.print("\tZ%: ");
    Serial.println(percentZ);*/
}