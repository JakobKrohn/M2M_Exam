#ifndef MQTT_H
#define MQTT_H

class Mqtt
{

    public:

        Mqtt(){};
        ~Mqtt(){};

        bool initialize();

        void update();

        void sendData(int bpm, int motion, float batteryLevel);

    private:

        bool connectWifi();

        bool connectClient();

        char * generateClientID();

        static void onMessage(const char * topic, byte * payload, unsigned int length);

};

#endif // MQTT_H