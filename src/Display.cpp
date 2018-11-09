#include <Arduino.h>
#include "Display.h"
#include "DeviceCredentials.h"
#include <U8g2lib.h>

// https://github.com/olikraus/u8g2

const int TOP_Y = 10;
const int BOTTOM_Y = 29;
const long heartBeatInterval = 200;
const long enableInterval = 50000;

unsigned long beatShownMillis = 0;

String displayedBpm, displayedMotion = "";

// unsigned int enableTime = 0; 
bool shouldUpdate = true;
bool displayEnabled = true;
bool nameEnabled = false;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void Display::initialize()
{
    Serial.print("\nInitializing Display ... ");

    u8g2.begin();
    u8g2.setPowerSave(0);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.setFontMode(0);

    Serial.println("done");
}

void Display::update()
{
    if (!shouldUpdate) { return; }

    unsigned long currentMillis = millis();

    // Manage beat
    if (currentMillis - beatShownMillis >= heartBeatInterval) {
        u8g2.setFont(u8g2_font_cursor_tf);
        u8g2.drawGlyph(114, 8, 0136);
        u8g2.sendBuffer();
        beatShownMillis = currentMillis;
    }
}

void Display::enableName(bool enabled)
{
    if (nameEnabled == enabled) {
        return;
    }

    nameEnabled = enabled;

    if (nameEnabled) {
        shouldUpdate = false;
        clearScreen();
        u8g2.setFont(u8g2_font_inb30_mf);
        u8g2.drawStr(0, BOTTOM_Y, DEVICE_ID);
        u8g2.sendBuffer();
        // Show name
    }
    else {
        shouldUpdate = true;
        clearScreen();
        setupAngleAndMovement();
    }
}

bool Display::isNameEnabled() const
{
    return nameEnabled;
}

void Display::enable(bool enabled)
{
    if (displayEnabled == enabled) {
        return;
    }
    
    displayEnabled = enabled;

    if (displayEnabled) {
        if (nameEnabled) {
            shouldUpdate = false;
            clearScreen();
            u8g2.setFont(u8g2_font_inb30_mf);
            u8g2.drawStr(0, BOTTOM_Y, DEVICE_ID);
            u8g2.sendBuffer();
        } else {
            shouldUpdate = true;
            setupAngleAndMovement();
        }
        u8g2.setPowerSave(0);
    }
    else {
        shouldUpdate = false;
        u8g2.setPowerSave(1);
    }
}

bool Display::isEnabled() const
{
    return displayEnabled;
}

void Display::clearScreen()
{
    u8g2.clearBuffer();
}

void Display::topLineMessage(const char * msg)
{
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.drawStr(0, BOTTOM_Y, "             ");
    u8g2.drawStr(0, TOP_Y, msg);
    u8g2.sendBuffer();
}

void Display::bottomLineMessage(const char * msg)
{
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.drawStr(0, BOTTOM_Y, "             ");
    u8g2.drawStr(0, BOTTOM_Y, msg);
    u8g2.sendBuffer();
}

void Display::setupAngleAndMovement()
{
    if (!shouldUpdate) { return; }

    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.clearBuffer();
    u8g2.drawStr(0, TOP_Y, "Pulse: ");
    u8g2.drawStr(0, BOTTOM_Y, "Motion: ");
}

void Display::singleMessage(const char * message)
{
    if (!shouldUpdate) { return; }

    u8g2.clearBuffer();
    u8g2.drawStr(0, (u8g2.getDisplayHeight() / 2) + 6, message);
    u8g2.sendBuffer();
}

void Display::motion(String motion)
{
    if (displayedMotion.equals(motion) || !shouldUpdate) {
        return;
    }

    displayedMotion = motion;
    
    addSpacing(motion);

    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.drawStr(70, BOTTOM_Y, motion.c_str());
    u8g2.sendBuffer();
}

void Display::bpm(String bpm)
{
    if (displayedBpm.equals(bpm) || !shouldUpdate) {
        return;
    }

    displayedBpm = bpm;
    
    addSpacing(bpm);
    
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.drawStr(60, TOP_Y, bpm.c_str());
    u8g2.sendBuffer();
}

void Display::beat()
{
    if (!shouldUpdate) { return; }

    // Font used: 
    // https://github.com/olikraus/u8g2/wiki/fntgrpx11
    u8g2.setFont(u8g2_font_cursor_tf);
    u8g2.drawGlyph(114, 8, 0137);
    u8g2.sendBuffer();
    beatShownMillis = millis();
}

// Private

void Display::addSpacing(String & str) const
{
    int offset = 5;
    int add = offset - str.length();

    for (int i = 0; i < add; i++) {
        str += " ";
    }
}

