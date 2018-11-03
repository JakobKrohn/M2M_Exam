#ifndef PULSE_H
#define PULSE_H

class Pulse
{

    public:

        Pulse(){};
        ~Pulse(){};

        void initialize();

        // Returns true if a beat is detected
        bool update();

    private:

        //

};

#endif // PULSE_H