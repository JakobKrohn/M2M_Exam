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

        String getMovementString() const;

    private:

        void calculatePercentage();

        void updateReadings();

};

#endif // MOTION_H