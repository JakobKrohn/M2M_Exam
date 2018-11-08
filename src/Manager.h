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

        void initialize();

        void start();

        void update();

    private: 

        Display _display;

        Motion _motion;

        Pulse _pulse;

        Mqtt _mqtt;

        void readBattery();

        static bool * getState();

        static void setState(int target, bool enable);

};

#endif // MANAGER_H