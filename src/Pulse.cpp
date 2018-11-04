#include <Arduino.h>
#include "Pulse.h"
#include "MAX30105.h"
#include "heartRate.h"

// https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library

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
    long irValue = particleSensor.getIR();

    if (irValue < 50000) {
        beatAvg = 0;
        return false;
    }

    if (checkForBeat(irValue) == true)
    {
        Serial.println("Beat!");
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
            beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
        }

        return true;
    }

    return false;
}

String Pulse::getBpmStr() const
{
    if (beatAvg == 0) {
        return "--";
    }

    return String(beatAvg);
}

int Pulse::getBpm() const
{
    return beatAvg;
}