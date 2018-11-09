#include <Arduino.h>
#include "Mqtt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ESP8266WiFi library:
// https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

// PubSubClient library:
// https://github.com/knolleary/pubsubclient 

// WIFI CREDENTIALS
const char * SSID = "K2-jakob";
const char * PASSWORD = "244466666";

// THINGSBOARD CREDENTIALS
const char * SERVER = "cloud.thingsboard.io";
const char * TOKEN = "vSiGoEgh9NprdCyGRmor";
const int PORT = 1883;

unsigned long lastSend = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

//boolean gpioState[] = {false, false};



bool Mqtt::initialize()
{
    // #define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback

    WiFi.begin(SSID, PASSWORD);
    if (connectWifi()) {
        client.setServer(SERVER, PORT);
        // https://github.com/knolleary/pubsubclient/issues/115
        client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->onMessage(topic, payload, length); });
        //client.setCallback(onMessage);
    }

    return false;
}

void Mqtt::setCallback(std::function<void(int, bool)> callback)
{
    setStateAA = callback;
}

void Mqtt::setCallback(std::function<bool(int)> callback)
{
    getStateAA = callback;
}

/*void Mqtt::setCallback( void setStateCallback(int target, bool enabled) )
{
    setState = setStateCallback;
}*/

// void Mqtt::setCallback( bool * getStateCallback() )
//template<class T> void Mqtt::setCallback( T* const object, void(T::* const mf)(int, bool) )
/*template<class T> void Mqtt::setCallback( T* const object, bool*(T::* const mf)() )
{
    //getState = getStateCallback;
}*/

void Mqtt::update(int bpm, int motion, float batteryLevel)
{
    if ( !client.connected() ) {
        connectClient();
    }

    if ( millis() - lastSend > 1000 ) { 
        sendData(bpm, motion, batteryLevel);
        lastSend = millis();
    }

    client.loop();
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

void Mqtt::sendData(int bpm, int motion, float batteryLevel)
{
    String payload = "{";
    payload += "\"bpm\":";
    payload += String(bpm);
    payload += ",";
    payload += "\"motion\":";
    payload += String(motion);
    payload += ",";
    payload += "\"battery\":";
    payload += String(batteryLevel);
    payload += "}";

    const char * attributes = payload.c_str();

    // Serial.println(attributes);

    client.publish("v1/devices/me/telemetry", attributes);
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

void Mqtt::onMessage(const char * topic, byte * payload, unsigned int length)
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
        
        //auto responsePayload = getStateString();

        client.publish(responseTopic.c_str(), getStateString().c_str());

    } 
    else if (methodName.equals("setGpioStatus")) {

        // Update GPIO status and reply
        setStateAA(data["params"]["pin"], data["params"]["enabled"]);

        //auto responsePayload = getStateString();

        //set_gpio_status(data["params"]["pin"], data["params"]["enabled"]);
        String responseTopic = String(topic);
        responseTopic.replace("request", "response");
        client.publish(responseTopic.c_str(), getStateString().c_str());
    }
}/* void set_gpio_status(int pin, boolean enabled) {
  if (pin == 0) {
    // Output GPIOs state
    //digitalWrite(GPIO0, enabled ? HIGH : LOW);
    // Update GPIOs state
    gpioState[0] = enabled;
  } else if (pin == 2) {
    // Output GPIOs state
    //digitalWrite(GPIO2, enabled ? HIGH : LOW);
    // Update GPIOs state
    gpioState[1] = enabled;
  }
}*/

String Mqtt::getStateString() const
{
    //auto states = getStateAA();

    bool rec = getStateAA(0);
    bool dis = getStateAA(1);

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    /*if (*(states + 0)) {
        Serial.println("0 is true");
        data[String(0)] = true;
    } 
    else {
        Serial.println("0 is false");
        data[String(0)] = false;
    }
    if (*(states + 1)) {
        Serial.println("1 is true");
        data[String(1)] = true;
    }
    else {
        Serial.println("1 is false");
        data[String(1)] = false;
    }*/

    data[String(0)] = rec ? true : false;
    data[String(1)] = dis ? true : false;

    char payload[256];
    data.printTo(payload, sizeof(payload));

    String strPayload = String(payload);

    Serial.print("Get gpio status: ");
    Serial.println(strPayload);

    return strPayload;
}
/*
String get_gpio_status() {
  // Prepare gpios JSON payload string
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data[String(0)] = gpioState[0] ? true : false;
  data[String(2)] = gpioState[1] ? true : false;
  char payload[256];
  data.printTo(payload, sizeof(payload));
  String strPayload = String(payload);
  Serial.print("Get gpio status: ");
  Serial.println(strPayload);
  return strPayload;
}*/

/*
Serial.println("Values fetched from manager in Mqtt: ");
        if (*(x+0)) {
            Serial.println("0 is true");
        } 
        if (*(x+1)) {
            Serial.println("1 is true");
        }
        //Serial.println(x);
        
*/