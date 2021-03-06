#ifndef THINGS_BOARD_H
#define THINGS_BOARD_H

class ThingsBoard
{

    public:

        ThingsBoard(){};
        ~ThingsBoard(){};

        // Connect to wifi and server
        bool initialize();

        // Set callback setState
        void setCallback(std::function<void(int, bool)> callback);

        // Set callback getState
        void setCallback(std::function<bool(int)> callback);

        // Check for connection, send data 
        void update(int bpm, int motion, float batteryLevel, bool faulty);

    private:

        std::function<void(int, bool)> setStateAA;

        std::function<bool(int)> getStateAA;

        //bool connectWifi();

        bool connectClient();

        void sendData(int bpm, int motion, float batteryLevel, bool faulty);

        char * generateClientID();

        void onMessage(const char * topic, byte * payload, unsigned int length);

        String getStateString() const;

};

#endif // THINGS_BOARD_H