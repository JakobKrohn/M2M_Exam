#include <Arduino.h>
#include "Pulse.h"
#include "MAX30105.h"
#include "heartRate.h"

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

MAX30105 particleSensor;

void Pulse::initialize()
{
    Serial.print("\nPulse initializing ... ");

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("\nMAX30105 was not found. Please check wiring/power. ");
        while (1) { yield(); }
    }
    Serial.println("Place your index finger on the sensor with steady pressure.");

    particleSensor.setup(); 
    particleSensor.setPulseAmplitudeRed(0x0A); 
    particleSensor.setPulseAmplitudeGreen(0); 

    Serial.println("done");
}

bool Pulse::update()
{
    

    return false;
}