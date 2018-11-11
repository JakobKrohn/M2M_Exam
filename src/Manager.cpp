#include <Arduino.h>
#include "Manager.h"
#include <Wire.h>

const int BATTERY_PIN = A0;
const long TUTORIAL_TIME = 50000;

bool _recording = false;

float _batteryLevel = 0;

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
    if (!_pulse.initialize()) {
        Serial.println("Failed at pulse");
        _display.clearScreen();
        _display.topLineMessage("Failed at");
        _display.bottomLineMessage("pulse sensor");
        while (true) { yield(); }
        //ESP.deepSleep(0);
        Serial.println("After sleep");
    }

    // Initialize motion sensor
    _display.bottomLineMessage("Init motion");
    _motion.initialize();

    // Initialize mqtt
    _display.bottomLineMessage("Init mqtt");
    if (!_mqtt.initialize()) {
        _display.topLineMessage("Failed at");
        _display.bottomLineMessage("wifi/mqtt");
        while (true) { yield(); }
        //ESP.deepSleep(0);
    }

    // Set callback for setState
    _mqtt.setCallback([this] (int target, bool enabled) { this->setState(target, enabled); });

    // Set callback for getState
    _mqtt.setCallback([this] (int target) -> bool { return this->getState(target); });

    _mqtt.update(0, 0, 0, true);
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
        readBattery();
        _mqtt.update(-1, -1, _batteryLevel, true);
        yield();
    }

    // Show pulse and motion for some time
    _display.setupAngleAndMovement();
    auto start = millis();
    while (millis() - start < TUTORIAL_TIME) {
        yield();
        update();
    }

    /*_display.clearScreen();
    _display.topLineMessage("Sit back");
    _display.bottomLineMessage("and enjoy!");

    // Can't delay here!
    delay(5000);
    // display.enable(false, time to turn off);

    _display.clearScreen();
    _display.setupAngleAndMovement();
    //_display.enable(false);*/
}

void Manager::update()
{
    readBattery();

    _display.update();
    _motion.update();

    if (_pulse.update()) {
        _display.beat();
    }

    _display.bpm(String(_pulse.getCurrentBpm()));
    _display.motion(_motion.getMovementString());

    _mqtt.update(
        _pulse.getCurrentBpm(), 
        _motion.getAverageMovement(), 
        _batteryLevel, 
        false);
}

// Private

void Manager::readBattery()
{
    unsigned int raw = analogRead(BATTERY_PIN);
    _batteryLevel = (raw / 1023.0) * 4.2;
}

bool Manager::getState(int target)
{
    switch (target) {
        case 0:
            return _display.isNameEnabled();
            break;
        case 1:
            return _display.isEnabled();
            break;
        case 2:
            break;
        default:
            break;
    }

    Serial.println("ERROR: get state");
    return false;
}

void Manager::setState(int target, bool enabled)
{
    Serial.println("\nSet state from MANAGER");
    Serial.print("Address of recording: ");
    Serial.println(*(&_recording));
    switch (target) {
        case 0:
            // Display
            _display.enableName(enabled);
            break;
        case 1:
            // Set display
            _display.enable(enabled);
            break;
        case 2: 
            // Not implemented
            Serial.println("Not implemented");
            break;
        default: 
            Serial.println("Not implemented");
            break;
    }
}