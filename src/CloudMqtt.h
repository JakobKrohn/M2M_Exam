#ifndef CLOUD_MQTT_H
#define CLOUD_MQTT_H

#include <Arduino.h>

namespace CM
{
    class CloudMqtt
    {

        public: 

            void initialize();

            bool shouldPowerOn();

            void update();

            bool shouldTurnOff();

        private:

            String message;

            void connect();

            void waitForMessage();

            void callback(char * topic, byte * payload, unsigned int length);

            char * generateClientID();

    };
}



#endif // CLOUD_MQTT_H