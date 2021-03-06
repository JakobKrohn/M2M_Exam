#include <Arduino.h>
#include "ThingsBoard.h"
#include "DeviceCredentials.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ESP8266WiFi library:
// https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

// PubSubClient library:
// https://github.com/knolleary/pubsubclient 

// WIFI CREDENTIALS
// const char * SSID = "K2-jakob";
// const char * PASSWORD = "244466666";

// THINGSBOARD CREDENTIALS
const char * SERVER = "cloud.thingsboard.io";
const int PORT = 1883;

unsigned long lastSend = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

bool ThingsBoard::initialize()
{
    client.setServer(SERVER, PORT);
    // https://github.com/knolleary/pubsubclient/issues/115
    client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->onMessage(topic, payload, length); });
    
    return true;

    // WiFi.begin(SSID, PASSWORD);
    /*if (connectWifi()) {
        client.setServer(SERVER, PORT);
        // https://github.com/knolleary/pubsubclient/issues/115
        client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->onMessage(topic, payload, length); });
        return true;
    }

    return false;*/
}

void ThingsBoard::setCallback(std::function<void(int, bool)> callback)
{
    setStateAA = callback;
}

void ThingsBoard::setCallback(std::function<bool(int)> callback)
{
    getStateAA = callback;
}

void ThingsBoard::update(int bpm, int motion, float batteryLevel, bool faulty)
{
    if ( !client.connected() ) {
        connectClient();
    }

    if ( millis() - lastSend > 1000 ) { 
        sendData(bpm, motion, batteryLevel, faulty);
        lastSend = millis();
    }

    client.loop();
}

// Private

/*bool ThingsBoard::connectWifi()
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
}*/

bool ThingsBoard::connectClient()
{
    /*if (!connectWifi()) {
        return false;
    }*/

    // Generate unique name
    auto clientID = generateClientID();

    int timeout = 5;
    int attempts = 0; 

    while (!client.connected()) {
        Serial.println("\nConnecting mqtt client");

        if (client.connect(clientID, DEVICE_TOKEN, NULL)) {
            Serial.println("Client connected");
            client.subscribe("v1/devices/me/rpc/request/+");
            Serial.println("Sending current GPIO status ...");
            client.publish("v1/devices/me/attributes", getStateString().c_str());
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

void ThingsBoard::sendData(int bpm, int motion, float batteryLevel, bool faulty)
{
    /*String act = (acting) ? "true" : "false";
    Serial.print("Acting: ");
    Serial.println(act);*/

    String payload = "{";
    payload += "\"bpm\":";
    payload += String(bpm);
    payload += ",";
    payload += "\"motion\":";
    payload += String(motion);
    payload += ",";
    payload += "\"battery\":";
    payload += String(batteryLevel);
    payload += ",";
    payload += "\"faulty\":";
    payload += faulty;
    payload += "}";

    const char * attributes = payload.c_str();

    Serial.println("");
    Serial.println(attributes);

    client.publish("v1/devices/me/telemetry", attributes);
}

char * ThingsBoard::generateClientID()
{
    static const char * alphanum = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";

    char * clientID = new char[10];

    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }

    clientID[8]='\0';

    return clientID;
}

void ThingsBoard::onMessage(const char * topic, byte * payload, unsigned int length)
{
    // https://thingsboard.io/docs/samples/esp8266/gpio/
    Serial.println("Message received");

    char json[length+1];
    strncpy(json, (char*) payload, length);
    json[length] = '\0';

    Serial.print("Topic: ");
    Serial.print(topic);
    Serial.print(" Message: ");
    Serial.println(json);

    // Decode JSON request
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject((char*) json);

    if (!data.success()) {
        Serial.println("Failed parsing json");
        return;
    }

    // Check request method
    String methodName = String((const char*)data["method"]);

    if (methodName.equals("getGpioStatus")) {

        // Reply with GPIO status
        String responseTopic = String(topic);
        responseTopic.replace("request", "response");
        
        client.publish(responseTopic.c_str(), getStateString().c_str());

    } 
    else if (methodName.equals("setGpioStatus")) {

        // Update GPIO status and reply
        setStateAA(data["params"]["pin"], data["params"]["enabled"]);

        //set_gpio_status(data["params"]["pin"], data["params"]["enabled"]);
        String responseTopic = String(topic);
        responseTopic.replace("request", "response");
        client.publish(responseTopic.c_str(), getStateString().c_str());
    }
}

String ThingsBoard::getStateString() const
{
    //auto states = getStateAA();

    bool rec = getStateAA(0);
    bool dis = getStateAA(1);

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    data[String(0)] = rec ? true : false;
    data[String(1)] = dis ? true : false;

    char payload[256];
    data.printTo(payload, sizeof(payload));

    String strPayload = String(payload);

    Serial.print("Get gpio status: ");
    Serial.println(strPayload);

    return strPayload;
}
