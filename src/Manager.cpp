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
    _pulse.initialize();

    // Initialize motion sensor
    _display.bottomLineMessage("Init motion");
    _motion.initialize();

    // Initialize mqtt
    _display.bottomLineMessage("Init mqtt");
    _mqtt.initialize();

    // Set callback for setState
    _mqtt.setCallback([this] (int target, bool enabled) { this->setState(target, enabled); });

    // Set callback for getState
    _mqtt.setCallback([this] (int target) -> bool { return this->getState(target); });

    _mqtt.update(0, 0, 0);
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

    /*while (!_pulse.update()) {
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
    }*/

    _display.clearScreen();
    _display.topLineMessage("Sit back");
    _display.bottomLineMessage("and enjoy!");

    // Can't delay here!
    //delay(5000);
    // display.enable(false, time to turn off);

    _display.clearScreen();
    _display.setupAngleAndMovement();
    //_display.enable(false);
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
        _batteryLevel);
}

// Private

void Manager::readBattery()
{
    unsigned int raw = analogRead(BATTERY_PIN);
    _batteryLevel = (raw / 1023.0) * 4.2;
}

bool Manager::getState(int target)
{
    //static bool gpioState[] = {_recording, this->_display.isEnabled()};
    Serial.println("\nGet state from MANAGER");

    switch (target) {
        case 0:
            Serial.print("Recording is: ");
            if (_recording) { 
                Serial.println("Enabled"); 
                return true;
            }
            else { 
                Serial.println("Disabled"); 
                return false;
            }
            break;
        case 1:
            Serial.print("Display is: ");
            if (_display.isEnabled()) {
                Serial.println("Enabled");
                return true;
            }
            else {
                Serial.println("Disabled");
                return false;
            }
            break;
        case 2:
            break;
        default:
            break;
    }

    Serial.println("ERROR: get state");
    return false;

    //return gpioState;
}

void Manager::setState(int target, bool enabled)
{
    Serial.println("\nSet state from MANAGER");
    Serial.print("Address of recording: ");
    Serial.println(*(&_recording));
    switch (target) {
        case 0:
            // Set recording
            Serial.print("Setting recording: ");
            if (enabled) { Serial.println("True"); }
            else { Serial.println("False"); }
            _recording = enabled;
            if (_recording) { Serial.println("Recording is now enabled"); }
            else { Serial.println("Recording is now disabled"); }
            break;
        case 1:
            // Set display
            Serial.print("Display was: ");
            if (_display.isEnabled()) { Serial.println("True"); }
            else { Serial.println("False"); }
            Serial.print("Setting display: ");
            if (enabled) { Serial.println("True"); }
            else { Serial.println("False"); }
            _display.enable(enabled);
            this->_display.enable(enabled);
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