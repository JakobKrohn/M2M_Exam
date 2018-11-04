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

  delay(2000);

  display.initialize();
  pulse.initialize();
  motion.initialize();

  display.setupAngleAndPulse();

}

auto counter = 0;
void loop() {

  auto loopStart = millis();

  display.update();
  
  if (pulse.update()) {
    display.beat();
  }

  motion.update();
  //Serial.println(motion.getMovement());

  display.bpm(pulse.getBpmStr());
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // readBattery();
    
    //display.angle(String(100));
    //display.beat();
    //display.bpm(String(100));

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

  auto loopStop = millis();
  //Serial.print("Loop time: ");
  //Serial.println(loopStop - loopStart);

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