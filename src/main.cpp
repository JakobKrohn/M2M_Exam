#include <Arduino.h>
#include "Manager.h"
/*#include <Wire.h>

#include "Display.h"
#include "Motion.h"
#include "Pulse.h"
#include "Mqtt.h"

const int BATTERY_PIN = A0;
const long interval = 1000;

unsigned long previousMillis = 0;
long timer = 0;

float batteryLevel = 0;

// GPIO 0
bool recording = false;

// GPIO 2
// Display active ?

Display display;
Motion motion;
Pulse pulse;
Mqtt mqtt;

void manageBattery();
void update();
bool * getState();
void setState(int target, bool state);
*/

Manager manager;

void setup() {
  manager.initialize();
  manager.start();
/*
  Serial.begin(115200);
  Wire.begin(D2, D1);

  delay(2000);

  display.initialize();
  display.topLineMessage("Please wait");

  display.bottomLineMessage("Init pulse");
  pulse.initialize();

  display.bottomLineMessage("Init motion");
  motion.initialize();

  display.bottomLineMessage("Init mqtt");
  //mqtt.initialize(setState, getState);
  mqtt.initialize();
  // Delay in tutorial - should connect here?
  // mqtt.update();

  // Greet user
  display.clearScreen();
  display.topLineMessage("Welcome!");
  delay(2000);

  // Explain how to use pulse sensor
  display.topLineMessage("Set finger on");
  display.bottomLineMessage("the red light");
  
  // Wait until user places finger on sensor
  //while (!pulse.update()) {
    // TODO go to sleep after some time here
    //yield();
    // ESP.deepSleep(0);
  //}

  // Show pulse and motion for some time
  display.setupAngleAndMovement();
  auto start = millis();
  while (millis() - start < 50000) {
    update();
  }

  Serial.println("Tutorial over");

  display.clearScreen();
  display.topLineMessage("Sit back");
  display.bottomLineMessage("and enjoy!");

  // Can't delay here!
  delay(5000);
  // display.enable(false, time to turn off);

  display.clearScreen();
  display.enable(false);
*/
}

void loop() {
  manager.update();
  //update();

}
/*
void manageBattery() {

  unsigned int raw = analogRead(BATTERY_PIN);

  batteryLevel = (raw / 1023.0) * 4.2;

  //Serial.print("Battery voltage: ");
  //Serial.println(batteryLevel);

}

void update() {

  manageBattery();

  display.update();
  motion.update();

  if (pulse.update()) {
    display.beat();
  }

  mqtt.update(pulse.getCurrentBpm(), motion.getAverageMovement(), batteryLevel);

  display.bpm(String(pulse.getCurrentBpm()));
  display.motion(motion.getMovementString());

}

bool * getState() {

  // Get gpio status: {"0":false,"2":false}
  static bool gpioState[] = {recording, display.isEnabled()};

  Serial.println("Get state");

  return gpioState;

}

void setState(int target, bool state) {

  Serial.println("Set State");

  if (target == 0) {
    recording = state;
  }
  else if (target == 2) {
    display.enable(state);
  }

}*/