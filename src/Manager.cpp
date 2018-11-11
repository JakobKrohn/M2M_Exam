#include <Arduino.h>
#include "Manager.h"
#include <Wire.h>

const int BATTERY_PIN = A0;
const long TUTORIAL_TIME = 10000;

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
    }

    // Set callback for setState
    _mqtt.setCallback([this] (int target, bool enabled) { this->setState(target, enabled); });

    // Set callback for getState
    _mqtt.setCallback([this] (int target) -> bool { return this->getState(target); });

    // Create a connection to thingsboard with fake data
    _mqtt.update(0, 0, 0, true);
}

void Manager::tutorial()
{
    // Greet user
    _display.clearScreen();
    _display.topLineMessage("Welcome!");

    // Display welcome message for 2 seconds
    bareUpdate(2000, false);

    // Explain how to use motion sensor
    _display.topLineMessage("Place the unit ");
    _display.bottomLineMessage("on your leg");

    bareUpdate(6000, false);

    waitForPulse();

    // Kudos to user
    _display.clearScreen();
    _display.topLineMessage("Good job!");

    bareUpdate(4000, false);

    _display.clearScreen();
    _display.topLineMessage("Sit back");
    _display.bottomLineMessage("and enjoy!");

    bareUpdate(2000, false);

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

    _mqtt.update(
        _pulse.getCurrentBpm(), 
        _motion.getAverageMovement(), 
        _batteryLevel, 
        !_pulse.isValid());
}

// Private

void Manager::waitForPulse()
{
    // Explain how to use pulse sensor
    _display.clearScreen();
    _display.topLineMessage("Strap finger ");
    _display.bottomLineMessage("on red light");

    // Wait until user places finger on motion sensor
    while (!_pulse.update()) {
        bareUpdate(true);
    }

    // Show pulse and motion for some time
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

void Manager::bareUpdate(int time, bool help) {
    auto start = millis();
    while (millis() - start < 2000) {
        readBattery();
        _pulse.update();
        _motion.update();
        _mqtt.update(
            _pulse.getCurrentBpm(), 
            _motion.getAverageMovement(), 
            _batteryLevel, 
            help);
        yield();
    }
}

void Manager::bareUpdate(bool help)
{
    readBattery();
    _pulse.update();
    _motion.update();
    _mqtt.update(
        _pulse.getCurrentBpm(), 
        _motion.getAverageMovement(), 
        _batteryLevel, 
        help);
    yield();
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