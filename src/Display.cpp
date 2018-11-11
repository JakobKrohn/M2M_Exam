#include <Arduino.h>
#include "Display.h"
#include "DeviceCredentials.h"
#include <U8g2lib.h>

// https://github.com/olikraus/u8g2

// Highest part of screen for writing
const int TOP_Y = 10;

// Lowest part of screen for writing
const int BOTTOM_Y = 29;

// How long beating hear i shown
const unsigned long HEART_BEAT_INTERVAL = 200;

// How long has beating heart been shown
unsigned long beatShownMillis = 0;

// Hold last displayed pulse and motion values
//  no need to print these again if they are matching
String displayedBpm, displayedMotion = "";

// Should the display be updated
bool shouldUpdate = true;

// Is display enabled
bool displayEnabled = true;

// Is name shown
bool nameEnabled = false;

// Library object
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

    manageBeatingHeart();
}

void Display::enableName(bool enabled)
{
    if (nameEnabled == enabled) {
        return;
    }

    nameEnabled = enabled;

    if (nameEnabled) {
        shouldUpdate = false;
        showName();
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
            showName();
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

void Display::manageBeatingHeart()
{
    unsigned long currentMillis = millis();

    // If beating heart has been displayed for a time, reset
    //  to empty heart. 
    if (currentMillis - beatShownMillis >= HEART_BEAT_INTERVAL) {
        u8g2.setFont(u8g2_font_cursor_tf);
        u8g2.drawGlyph(114, 8, 0136);
        u8g2.sendBuffer();
        beatShownMillis = currentMillis;
    }
}

void Display::showName()
{
    clearScreen();
    u8g2.setFont(u8g2_font_inb30_mf);
    u8g2.drawStr(0, BOTTOM_Y, DEVICE_ID);
    u8g2.sendBuffer();
}

void Display::addSpacing(String & str) const
{
    int offset = 5;
    int add = offset - str.length();

    for (int i = 0; i < add; i++) {
        str += " ";
    }
}

