#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{

    public:

        Display(){};
        ~Display(){};

        void initialize();

        void update();

        void setupAngleAndPulse();

        void singleMessage(const char * message);

        void angle(String angle);

        void bpm(String bpm);

        void beat();

    private:

        //

};

#endif // DISPLAY_H