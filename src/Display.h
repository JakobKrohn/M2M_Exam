#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{

    public:

        Display(){};
        ~Display(){};

        // Setup display. 
        void initialize();

        // Handles heart beat signals. 
        void update();

        // Shows device name in large font. 
        // Used to locate devices. 
        void enableName(bool enabled);

        // Gives feedback to admin 
        //  if the name is displayed. 
        bool isNameEnabled() const;

        // Turn on or off display. 
        // Turn display off during test 
        //  so it doesn't disrupt the user. 
        // Turn on to teach user how to use
        //  how to use the device. 
        void enable(bool enabled);

        // Gives feedback to admin 
        //  if the display is on or off. 
        bool isEnabled() const;

        // Clears the screen. 
        void clearScreen();

        // Prints message at first line. 
        void topLineMessage(const char * message);

        // Prints message at second line. 
        void bottomLineMessage(const char * message);

        // Sets up pulse and movement format. 
        // Call this before printing pulse/movement/hearbeat
        void pulseAndMovement();

        // Display current movement
        void motion(String motion);

        // Display current pulse
        void bpm(String bpm);

        // Show a beating heart
        //  update method resets this
        void beat();

    private:

        void manageBeatingHeart();

        void showName();

        // Add spacing to string
        //  so previous string isn't showed
        void addSpacing(String & str) const;

};

#endif // DISPLAY_H