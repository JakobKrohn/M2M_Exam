#ifndef MOTION_H
#define MOTION_H

class Motion 
{

    public:

        Motion(){};
        ~Motion(){};

        void initialize();

        void update();

        int getAverageMovement() const;

        int getCurrentMovement() const;

    private:

        void calculatePercentage();

};

#endif // MOTION_H