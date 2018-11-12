#include "CloudMqtt.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

namespace CM
{
    WiFiClient wifiClient;
    PubSubClient client(wifiClient);

    void CloudMqtt::initialize()
    {
        Serial.println("Initializing cloud mqtt");

        client.setServer("m15.cloudmqtt.com", 17350);
        client.setCallback([this] (char * topic, byte * payload, unsigned int length) { this->callback(topic, payload, length); });

        connect();
    }

    bool CloudMqtt::shouldPowerOn()
    {
        connect();

        waitForMessage();

        if (message.equals("On")) {
            // Server says power ON!
            return true;
        }
        else {
            // Server says power OFF!
            return false;
        }
    }

    void CloudMqtt::update()
    {
        connect();
        client.loop();
    }

    bool CloudMqtt::shouldTurnOff()
    {
        if (message.equals("On")) {
            // Server says power ON!
            return false;
        }
        else {
            // Server says power OFF!
            return true;
        }
    }

    // Private

    void CloudMqtt::connect()
    {
        while (!client.connected()) 
        {
            Serial.print("Attempting MQTT connection...");
            
            // Generate ClientID
            auto clientID = generateClientID();

            Serial.println("Client connecting");

            // Connect to the MQTT broker
            if (client.connect(clientID, "WemosUnit", "2qQ09u2m8bb4ggZL7WWT")) 
            {
                Serial.println("connected");

                client.subscribe("Power");

                // This is not tested !
                delete clientID;

                return;
            }
            else
            {
                Serial.print("failed, rc=");
                // Print to know why the connection failed.
                // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");

                delay(2000);
            }

            delete(clientID);
        }
    }

    void CloudMqtt::waitForMessage()
    {
        Serial.println("Waiting for messages");
        while (1) {
            
            client.loop();
            yield();

            // We got a message!
            if (message != "") {
                return;
            }

            Serial.print(".");
            delay(50);
        }
        Serial.println("");
    }

    void CloudMqtt::callback(char * topic, byte * payload, unsigned int length)
    {
        Serial.print("\nMessage arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < (int)length; i++) {
            Serial.print((char)payload[i]);
        }
        Serial.println();

        // Convert to strings
        String strTopic = String(topic);
        String strPayload = "";
        for (int i = 0; i < (int)length; i++) {
            strPayload += (char)payload[i];
        }

        // Save the message
        message = strPayload;
    }

    char * CloudMqtt::generateClientID()
    {
        static const char * alphanum = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";

        char * clientID = new char[10];

        for (int i = 0; i < 8; i++) {
            clientID[i] = alphanum[random(51)];
        }

        clientID[8]='\0';

        return clientID;
    }
}

