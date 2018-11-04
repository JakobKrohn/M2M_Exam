#include <Arduino.h>
#include "Display.h"
#include <U8g2lib.h>

// https://github.com/olikraus/u8g2

const int TOP_Y = 10;
const int BOTTOM_Y = 29;
const long heartBeatInterval = 200;

unsigned long beatShownMillis = 0;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void Display::initialize()
{
    Serial.print("\nInitializing Display ... ");

    u8g2.begin();

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.setFontMode(0);
    u8g2.drawStr(0, (u8g2.getDisplayHeight() / 2) + 6, "Initializing");
    u8g2.sendBuffer();

    Serial.println("done");
}

void Display::update()
{
    unsigned long currentMillis = millis();

    if (currentMillis - beatShownMillis >= heartBeatInterval) {
        u8g2.setFont(u8g2_font_cursor_tf);
        u8g2.drawGlyph(114, 8, 0136);
        u8g2.sendBuffer();
        //Serial.print("Pulse shown: ");
        //Serial.println(currentMillis - beatShownMillis);
        beatShownMillis = currentMillis;
    }
}

void Display::setupAngleAndMovement()
{
    u8g2.clearBuffer();
    u8g2.drawStr(0, TOP_Y, "Pulse: ");
    u8g2.drawStr(0, BOTTOM_Y, "Motion: ");
}

void Display::singleMessage(const char * message)
{
    u8g2.clearBuffer();
    u8g2.drawStr(0, (u8g2.getDisplayHeight() / 2) + 6, message);
    u8g2.sendBuffer();
}

void Display::motion(String motion)
{

    int offset = 4;
    int add = offset - motion.length();
    for (int i = 0; i < add; i++) {
        motion += " ";
    }

    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.drawStr(70, BOTTOM_Y, motion.c_str());
    u8g2.sendBuffer();
}

void Display::bpm(String bpm)
{
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.drawStr(65, TOP_Y, bpm.c_str());
    u8g2.sendBuffer();
}

void Display::beat()
{
    // https://github.com/olikraus/u8g2/wiki/fntgrpx11
    // empty heart -> 0136
    // bank heart -> 0137
    u8g2.setFont(u8g2_font_cursor_tf);
    u8g2.drawGlyph(114, 8, 0137);
    u8g2.sendBuffer();
    beatShownMillis = millis();
}