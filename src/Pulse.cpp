#include <Arduino.h>
#include "Pulse.h"
#include "MAX30105.h"
#include "heartRate.h"

// https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library

const int MEASURE_SIZE = 8;

// Stores readings, used to calculate average bpm
std::vector<byte> measurements(MEASURE_SIZE);
byte measureCounter = 0;

float lastBeat = 0;       // Stores time since last beat

float measuredBpm = 0;    // The actual measured bpm
int currentBpm = 0;       // The calculated bpm, average of last 8 readings

MAX30105 sensor;

bool Pulse::initialize()
{
    Serial.print("\nPulse initializing ... ");

    if (!sensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("\nMAX30105 was not found. Please check wiring/power. ");
        return false;
    }

    sensor.setup(); 

    Serial.println("done");
    return true;
}

bool Pulse::update()
{
    long irValue = sensor.getIR();

    if (irValue < 50000) {
        currentBpm = 0;
        return false;
    }

    if (checkForBeat(irValue)) {
        long timeSinceLastBeat = millis() - lastBeat;
        lastBeat = millis();

        calculateCurrentBpm(timeSinceLastBeat);

        return true;
    }

    return false;
}

int Pulse::getCurrentBpm() const
{
    return currentBpm;
}

bool Pulse::isValid() const
{
    long irValue = sensor.getIR();

    if (irValue < 50000) {
        return false;
    }

    return true;
}

// Private

void Pulse::calculateCurrentBpm(int timeSinceLastBeat)
{
    measuredBpm = 60 / (timeSinceLastBeat / 1000.0);

    if (measuredBpm < 255 && measuredBpm > 20) {

        // Store reading in vector
        measurements[measureCounter++] = (byte)measuredBpm;

        // If measureCounter is measure_size set it back to 0
        measureCounter %= MEASURE_SIZE;

        // Take average of readings
        currentBpm = 0;
        for (byte x = 0; x < MEASURE_SIZE; x++) {
            currentBpm += measurements[x];
        }

        currentBpm /= MEASURE_SIZE;

    }
    
}