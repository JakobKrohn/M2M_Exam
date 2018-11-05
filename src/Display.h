#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{

    public:

        Display(){};
        ~Display(){};

        void initialize();

        void update();

        void setupAngleAndMovement();

        void singleMessage(const char * message);

        void motion(String motion);

        void bpm(String bpm);

        void beat();

    private:

        void addSpacing(String & str) const;

};

#endif // DISPLAY_H