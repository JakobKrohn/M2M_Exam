#include <Arduino.h>
#include <Wire.h>

#include "Display.h"
#include "Motion.h"
#include "Pulse.h"

const int BATTERY_PIN = A0;
const long interval = 1000;

unsigned long previousMillis = 0;
long timer = 0;

Display display;
Motion motion;
Pulse pulse;

float readBattery();

void setup() {

  Serial.begin(115200);
  Wire.begin(D2, D1);

  display.initialize();
  /*delay(2000);
  motion.initialize();
  display.singleMessage("Motion OK");
  delay(2000);
  pulse.initialize();
  display.singleMessage("Pulse OK");
  delay(2000);*/

  display.setupAngleAndPulse();

}

auto counter = 0;
void loop() {

  //mpu6050.update();

  display.update();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // readBattery();
    
    //display.angle(String(100));
    display.beat();

    previousMillis = currentMillis;
  }

  /*Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.print(mpu6050.getAngleZ());*/
  //Serial.print(", Avg BPM=");
  //Serial.println(beatAvg);

/*  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
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
  }*/
}

float readBattery() {

  unsigned int raw = analogRead(BATTERY_PIN);
  float batterVoltage = (raw / 1023.0) * 4.2;

  Serial.print("Battery voltage: ");
  Serial.println(batterVoltage);

  return batterVoltage;

}

/*

Notater: 

- Hvis MPU6050 sensoren ikke gir fornuftige resultater må den kobles av og på igjen og wemos resettes. 

*/