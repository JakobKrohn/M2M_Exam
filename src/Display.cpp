#include <Arduino.h>
#include "Display.h"
#include <U8g2lib.h>

// https://github.com/olikraus/u8g2

const int TOP_Y = 10;
const int BOTTOM_Y = 29;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const long heartBeatInterval = 200;
unsigned long prevMillis = 0;
bool heartBeatShown = false;

void Display::initialize()
{
    Serial.print("\nInitializing Display ... ");

    u8g2.begin();

    u8g2.clearBuffer();
    //u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setFont(u8g2_font_9x15_mf);
    // u8g2_font_ncenB14_tr
    u8g2.setFontMode(0);
    //u8g2.setDrawColor(0);
    u8g2.drawStr(0, (u8g2.getDisplayHeight() / 2) + 6, "Display OK");
    u8g2.sendBuffer();

    Serial.println("done");
}

void Display::update()
{
/*    if (heartBeatShown) {
        u8g2.setFont(u8g2_font_cursor_tf);
        u8g2.drawGlyph(114, 8, 0136);
        u8g2.sendBuffer();
    }
*/
    unsigned long currentMillis = millis();
    if (currentMillis - prevMillis >= heartBeatInterval) {
        u8g2.setFont(u8g2_font_cursor_tf);
        u8g2.drawGlyph(114, 8, 0136);
        u8g2.sendBuffer();
        prevMillis = currentMillis;
    }
}

void Display::setupAngleAndPulse()
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

void Display::angle(String angle)
{
    u8g2.drawStr(70, BOTTOM_Y, angle.c_str());
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
}