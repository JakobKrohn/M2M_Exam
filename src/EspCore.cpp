#include "EspCore.h"
#include <ESP8266WiFi.h>

const char * SSID = "Student";
const char * PASSWORD = "Student1914";

void EspCore::connectWifi()
{
    Serial.println("Connecting wifi");

    WiFi.begin(SSID, PASSWORD);

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Wifi already connected");
        return;
    }

    Serial.print("Wifi connecting ");

    WiFi.begin(SSID, PASSWORD);

    autoConnect();
}

void EspCore::autoConnect()
{
    int timeout = 50;
    int attempts = 0;

    while (!WiFi.status() == WL_CONNECTED) {
        Serial.print(".");
        
        if (attempts++ >= timeout) {
            Serial.println(F("\nE: Could not connect to Wifi, going to sleep"));
            
            // Sleep for 1 minutes
            sleep(1);
        }

        delay(500);
    }

    Serial.println("\nwifi connected!");
}

void EspCore::sleep(int minutes)
{
    int microseconds = minutes * 60000000;

    Serial.print("\nSleeping for ");
    Serial.print(minutes);
    Serial.print(" minutes (");
    Serial.print(microseconds);
    Serial.println(" microseconds)");

    ESP.deepSleep(microseconds);
}