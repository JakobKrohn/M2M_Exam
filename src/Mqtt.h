#ifndef MQTT_H
#define MQTT_H

class Mqtt
{

    public:

        Mqtt(){};
        ~Mqtt(){};

        bool initialize();

        void setCallback(std::function<void(int, bool)> callback);

        void setCallback(std::function<bool*(int)> callback);

        void setCallback( void setStateCallback(int target, bool enabled) );

        // void setCallback( bool * getState() );
        //template<class T> void setCallback( T* const object, bool*(T::* const mf)() );

        void update(int bpm, int motion, float batteryLevel);

    private:

        bool connectWifi();

        bool connectClient();

        void sendData(int bpm, int motion, float batteryLevel);

        char * generateClientID();

        void onMessage(const char * topic, byte * payload, unsigned int length);

        //virtual bool * getState();

        //virtual void setState(int target, bool enable);

        std::function<void(int, bool)> setStateAA;

        std::function<bool*(int)> getStateAA;

        //void (*setState)(int target, bool state);

        //bool * (*getState)();

};

#endif // MQTT_H