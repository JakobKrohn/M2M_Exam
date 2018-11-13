#ifndef MANAGER_H
#define MANAGER_H

#include "EspCore.h"
#include "CloudMqtt.h"
#include "Display.h"
#include "Motion.h"
#include "Pulse.h"
#include "ThingsBoard.h"

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

        EspCore _espCore;

        CM::CloudMqtt _cloudMqtt;

        Display _display;

        Motion _motion;

        Pulse _pulse;

        ThingsBoard _thingsBoard;

        void waitForPulse();
        
        void readBattery();

        // Returns current state from a target. 
        // Targets implemented are: 
        //      - Display enabled or disabled
        //      - Display name or not
        bool getState(int target);

        // Sets the state to a target. 
        // Either show name, or turn on/off display
        void setState(int target, bool enable);

};

#endif // MANAGER_H