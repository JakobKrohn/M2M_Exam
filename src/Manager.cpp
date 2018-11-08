#include <Arduino.h>
#include "Manager.h"
#include <Wire.h>

const int BATTERY_PIN = A0;
const long TUTORIAL_TIME = 50000;

float batteryLevel = 0;

void Manager::initialize()
{
    Serial.begin(115200);
    Wire.begin(D2, D1);

    delay(2000);

    // Initialize display
    _display.initialize();
    _display.topLineMessage("Please wait");

    // Initialize pulse sensor
    _display.bottomLineMessage("Init pulse");
    _pulse.initialize();

    // Initialize motion sensor
    _display.bottomLineMessage("Init motion");
    _motion.initialize();

    // Initialize mqtt
    _display.bottomLineMessage("Init mqtt");
    _mqtt.initialize();
    _mqtt.update(0, 0, 0);
    _mqtt.setCallback(setState);
    _mqtt.setCallback(getState);
}

void Manager::start()
{
    // Greet user
    _display.clearScreen();
    _display.topLineMessage("Welcome!");
    delay(2000);

    // Explain how to use pulse sensor
    _display.topLineMessage("Set finger on");
    _display.bottomLineMessage("the red light");

    while (!_pulse.update()) {
        // TODO go to sleep after some time here
        yield();
        // ESP.deepSleep(0);
    }

    // Show pulse and motion for some time
    _display.setupAngleAndMovement();
    auto start = millis();
    while (millis() - start < TUTORIAL_TIME) {
        yield();
        update();
    }

    _display.clearScreen();
    _display.topLineMessage("Sit back");
    _display.bottomLineMessage("and enjoy!");

    // Can't delay here!
    delay(5000);
    // display.enable(false, time to turn off);

    _display.clearScreen();
    _display.enable(false);
}

void Manager::update()
{
    readBattery();

    _display.update();
    _motion.update();

    if (_pulse.update()) {
        _display.beat();
    }

    _mqtt.update(
        _pulse.getCurrentBpm(), 
        _motion.getAverageMovement(), 
        batteryLevel);

    _display.bpm(String(_pulse.getCurrentBpm()));
    _display.motion(_motion.getMovementString());
}

// Private

void Manager::readBattery()
{
    unsigned int raw = analogRead(BATTERY_PIN);
    batteryLevel = (raw / 1023.0) * 4.2;
}

bool * Manager::getState()
{
    Serial.println("Get state manager");
}

void Manager::setState(int target, bool enabled)
{
    Serial.println("Set state manager");
}