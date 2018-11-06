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

float batteryLevel = 0;

Display display;
Motion motion;
Pulse pulse;
Mqtt mqtt;

void manageBattery();

void setup() {

  Serial.begin(115200);
  Wire.begin(D2, D1);

  delay(2000);

  display.initialize();
  display.singleMessage("Init pulse");
  pulse.initialize();
  display.singleMessage("Init motion");
  motion.initialize();
  display.singleMessage("Init mqtt");
  mqtt.initialize();
  mqtt.update();

  display.setupAngleAndMovement();

}

auto counter = 0;
void loop() {

  //auto loopStart = millis();

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

    mqtt.sendData(pulse.getCurrentBpm(), motion.getAverageMovement(), batteryLevel);

    previousMillis = currentMillis;
  }

  //auto loopStop = millis();
  //auto loopTime = loopStop - loopStart;
  //Serial.print("Loop time: ");
  //Serial.println(loopStop - loopStart);

}

void manageBattery() {

  unsigned int raw = analogRead(BATTERY_PIN);
  //float batterVoltage = (raw / 1023.0) * 4.2;
  batteryLevel = (raw / 1023.0) * 4.2;

  //batteryLevel = batterVoltage;

  Serial.print("Battery voltage: ");
  Serial.println(batteryLevel);

  if (batteryLevel < 4.2) {
    Serial.println("Off dislay");
    display.enable(false);
  } else {
    display.enable(true);
  }

}

/*

Notater: 

- Hvis MPU6050 sensoren ikke gir fornuftige resultater må den kobles av og på igjen og wemos resettes. 

*/

