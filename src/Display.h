#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{

    public:

        Display(){};
        ~Display(){};

        void initialize();

        void update();

        void enable(bool enabled);

        void clearScreen();

        void topLineMessage(const char * message);

        void bottomLineMessage(const char * message);

        void setupAngleAndMovement();

        void singleMessage(const char * message);

        void motion(String motion);

        void bpm(String bpm);

        void beat();

    private:

        void addSpacing(String & str) const;

        bool isEnabled() const;

};

#endif // DISPLAY_H