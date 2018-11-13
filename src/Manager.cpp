#include <Arduino.h>
#include "Manager.h"
#include <Wire.h>

const int BATTERY_PIN = A0;
const long TUTORIAL_TIME = 10000;

//bool _recording = false;

float _batteryLevel = 0;

void Manager::initialize()
{
    Serial.begin(115200);
    pinMode(D1, OUTPUT);
    delay(1000);

    _espCore.connectWifi();
    _cloudMqtt.initialize();

    // Check if device should connect or not
    bool power = _cloudMqtt.shouldPowerOn();

    if (!power) {
        Serial.println("Sleeping for a minute");
        _espCore.sleep(1);
    }

    digitalWrite(D1, HIGH);
    //Wire.begin(D2, D1);
    Wire.begin(12, 14);

    delay(4000);

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
    }

    // Initialize motion sensor
    _display.bottomLineMessage("Init motion");
    _motion.initialize();

    // Initialize mqtt
    _display.bottomLineMessage("Init mqtt");
    if (!_thingsBoard.initialize()) {
        _display.topLineMessage("Failed at");
        _display.bottomLineMessage("wifi/mqtt");
        while (true) { yield(); }
    }

    // Set callback for setState
    _thingsBoard.setCallback([this] (int target, bool enabled) { this->setState(target, enabled); });

    // Set callback for getState
    _thingsBoard.setCallback([this] (int target) -> bool { return this->getState(target); });

    // Create a connection to thingsboard with fake data
    _thingsBoard.update(0, 0, 0, true);
}

void Manager::tutorial()
{
    Serial.println("Starting tutorial");
    // Greet user
    _display.clearScreen();
    _display.topLineMessage("Welcome!");
    _display.enableUpdate(false);

    // Display welcome message for 2 seconds
    //bareUpdate(5000, false);
    auto start = millis();
    while (millis() - start < 5000) {
        update();
        yield();
    }

    // Explain how to use motion sensor
    _display.topLineMessage("Place the unit ");
    _display.bottomLineMessage("on your leg");

    //bareUpdate(6000, false);
    start = millis();
    while (millis() - start < 10000) {
        update();
        yield();
    }

    waitForPulse();

    // Kudos to user
    _display.clearScreen();
    _display.enableUpdate(false);
    _display.topLineMessage("Good job!");

    //bareUpdate(4000, false);
    start = millis();
    while (millis() - start < 5000) {
        update();
        yield();
    }

    _display.clearScreen();
    _display.enableUpdate(false);
    _display.topLineMessage("Sit back");
    _display.bottomLineMessage("and enjoy!");

    //bareUpdate(2000, false);
    start = millis();
    while (millis() - start < 4000) {
        update();
        yield();
    }

    _display.enableUpdate(true);
    _display.clearScreen();
    _display.pulseAndMovement();
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

    _thingsBoard.update(
        _pulse.getCurrentBpm(), 
        _motion.getAverageMovement(), 
        _batteryLevel, 
        !_pulse.isValid());

    _cloudMqtt.update();
    if (_cloudMqtt.shouldTurnOff()) {
        Serial.println("Server says power down");
        _espCore.sleep(1);
    }
}

// Private

void Manager::waitForPulse()
{
    // Explain how to use pulse sensor
    _display.clearScreen();
    _display.enableUpdate(false);
    _display.topLineMessage("Strap finger ");
    _display.bottomLineMessage("on red light");

    // Wait until user places finger on motion sensor
    while (!_pulse.update()) {
        update();
        //bareUpdate(true);
    }

    // Show pulse and motion for some time
    _display.enableUpdate(true);
    _display.pulseAndMovement();
    auto start = millis();
    while (millis() - start < TUTORIAL_TIME) {
        yield();
        update();
        if (!_pulse.isValid()) {
            waitForPulse();
        }
    }
}

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

    return false;
}

void Manager::setState(int target, bool enabled)
{
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