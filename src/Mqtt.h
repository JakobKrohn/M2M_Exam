#ifndef MQTT_H
#define MQTT_H

class Mqtt
{

    public:

        Mqtt(){};
        ~Mqtt(){};

        bool initialize();

        void update();

        void sendData(int bpm, int motion);

    private:

        bool connectWifi();

        bool connectClient();

        char * generateClientID();

};

#endif // MQTT_H