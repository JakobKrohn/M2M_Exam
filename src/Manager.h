#ifndef MANAGER_H
#define MANAGER_H

#include "Display.h"
#include "Motion.h"
#include "Pulse.h"
#include "Mqtt.h"

class Manager
{

    public:

        Manager(){};
        ~Manager(){};

        // Initialize all components
        void initialize();

        // Start tutorial and wait until user 
        //  places finger on pulse sensor
        void tutorial();

        // Update all components
        void update();

    private: 

        Display _display;

        Motion _motion;

        Pulse _pulse;

        Mqtt _mqtt;

        // Keep connection with thingsboard
        //  send faulty data, while waiting for user 
        //  to accomplish something. 
        void bareUpdate(int time, bool help);

        void bareUpdate(bool help);

        void waitForPulse();

        void readBattery();

        bool getState(int target);

        void setState(int target, bool enable);

};

#endif // MANAGER_H