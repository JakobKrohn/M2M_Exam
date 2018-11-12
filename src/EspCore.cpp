#include "EspCore.h"
#include <ESP8266WiFi.h>

const char * SSID = "K2-jakob";
const char * PASSWORD = "244466666";

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

    int timeout = 50;
    int attempts = 0;

    while (!WiFi.status() == WL_CONNECTED) {
        Serial.print(".");
        
        if (attempts++ >= timeout) {
            Serial.println(F("\nE: Could not connect to Wifi, going to sleep"));
            
            // Sleep for 15 minutes
            sleep(15);
        }

        delay(500);
    }

    Serial.println("\nConnected!");
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