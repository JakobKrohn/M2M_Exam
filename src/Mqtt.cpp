#include <Arduino.h>
#include "Mqtt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ESP8266WiFi library:
// https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

// PubSubClient library:
// https://github.com/knolleary/pubsubclient 

// WIFI CREDENTIALS
const char * SSID = "K2-jakob";
const char * PASSWORD = "244466666";

// THINGSBOARD CREDENTIALS
const char * SERVER = "demo.thingsboard.io";
const char * TOKEN = "6prJoiaVd1l26vipj7RR";
const int PORT = 1883;

unsigned long lastSend = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

bool Mqtt::initialize()
{
    if (connectWifi()) {
        client.setServer(SERVER, PORT);
    }

    return false;
}

void Mqtt::update()
{
    if ( !client.connected() ) {
        connectClient();
    }

    if ( millis() - lastSend > 1000 ) { 
        lastSend = millis();
    }

    client.loop();
}

void Mqtt::sendData(int bpm, int motion)
{
    String payload = "{";
    payload += "\"bpm\":";
    payload += String(bpm);
    payload += ",";
    payload += "\"motion\":";
    payload += String(motion);
    payload += "}";

    const char * attributes = payload.c_str();

    client.publish("v1/devices/me/telemetry", attributes);
}

// Private

bool Mqtt::connectWifi()
{
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Wifi already connected");
        return true;
    }

    Serial.print("Wifi connecting ");

    WiFi.begin(SSID, PASSWORD);

    int timeout = 50;
    int attempts = 0;

    while (!WiFi.status() == WL_CONNECTED) {
        Serial.print(".");
        
        if (attempts++ >= timeout) {
            Serial.println(F("\nE: Could not connect to Wifi, going to sleep"));
            return false;
        }

        delay(500);
    }

    Serial.println("\nConnected!");
    return true;
}

bool Mqtt::connectClient()
{
    if (!connectWifi()) {
        return false;
    }

    // Generate unique name
    auto clientID = generateClientID();

    int timeout = 5;
    int attempts = 0; 

    while (!client.connected()) {
        Serial.println("Connecting mqtt client");

        if (client.connect(clientID, TOKEN, NULL)) {
            Serial.println("Client connected");
            break;
        }
        else {
            Serial.print("failed, rc=");
            // Print to know why the connection failed.
            // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");

            delete clientID;

            if (attempts++ >= timeout) {
                return false;
            }

            clientID = generateClientID();

            delay(5000);
        }
    }

    delete clientID;

    return true;

}

char * Mqtt::generateClientID()
{
    static const char * alphanum = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";

    char * clientID = new char[10];

    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }

    clientID[8]='\0';

    return clientID;
}