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

        String getBpmStr() const;
        
        int getBpm() const;

    private:

        //

};

#endif // PULSE_H