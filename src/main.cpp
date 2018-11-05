#include <Arduino.h>
#include <Wire.h>

#include "Display.h"
#include "Motion.h"
#include "Pulse.h"
#include "Mqtt.h"

const int BATTERY_PIN = A0;
const long interval = 1000;

unsigned long previousMillis = 0;
long timer = 0;

Display display;
Motion motion;
Pulse pulse;
Mqtt mqtt;

float manageBattery();

void setup() {

  Serial.begin(115200);
  Wire.begin(D2, D1);

  delay(2000);

  display.initialize();
  pulse.initialize();
  motion.initialize();
  mqtt.initialize();
  mqtt.update();

  display.setupAngleAndMovement();

}

auto counter = 0;
void loop() {

  auto loopStart = millis();

  display.update();
  
  if (pulse.update()) {
    display.beat();
  }

  motion.update();

  display.bpm(String(pulse.getCurrentBpm()));
  display.motion(String(motion.getCurrentMovement()));

  mqtt.update();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {

    manageBattery();

    mqtt.sendData(pulse.getCurrentBpm(), motion.getAverageMovement());

    previousMillis = currentMillis;
  }

  auto loopStop = millis();
  auto loopTime = loopStop - loopStart;
  //Serial.print("Loop time: ");
  //Serial.println(loopStop - loopStart);

}

float manageBattery() {

  unsigned int raw = analogRead(BATTERY_PIN);
  float batterVoltage = (raw / 1023.0) * 4.2;

  Serial.print("Battery voltage: ");
  Serial.println(batterVoltage);

  if (batterVoltage < 3.3) {
    display.enable(0);
  }

  return batterVoltage;

}

/*

Notater: 

- Hvis MPU6050 sensoren ikke gir fornuftige resultater må den kobles av og på igjen og wemos resettes. 

*/