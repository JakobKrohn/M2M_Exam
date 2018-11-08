#ifndef MQTT_H
#define MQTT_H

class Mqtt
{

    public:

        Mqtt(){};
        ~Mqtt(){};

        bool initialize();

        void setCallback( void setStateCallback(int target, bool enabled) );

        void setCallback( bool * getState() );

        void update(int bpm, int motion, float batteryLevel);

    private:

        bool connectWifi();

        bool connectClient();

        void sendData(int bpm, int motion, float batteryLevel);

        char * generateClientID();

        void onMessage(const char * topic, byte * payload, unsigned int length);

        void (*setState)(int target, bool state);

        bool * (*getState)();

};

#endif // MQTT_H