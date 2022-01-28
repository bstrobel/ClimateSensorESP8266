#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <string>
#include "MCDisplay.h"

// We use full buffer mode. Seems to be working well with ESP8266
// We use I2C SW mode. There seems to be no I2C HW in an ESP8266, at least it doesn't work for me.
U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, D1, D2);

MCDisplay::MCDisplay()
{
    display.begin();
}


MCDisplay::~MCDisplay()
{
    display.flush();
}

void MCDisplay::enable()
{

}

void MCDisplay::disable()
{

}

void MCDisplay::displayWeatherData(MeasureClimate &mc)
{

    display.setFont(u8g2_font_profont22_tf); // width 12, height 22
    display.clearBuffer();
    display.setCursor(0, display.getMaxCharHeight()-1);
    display.print(mc.temperature, 1);
    display.print("C");
    display.setCursor(0, 2*(display.getMaxCharHeight()-1));
    display.print(mc.pressure_rel, 1);
    display.print(F("hPa"));
    display.setCursor(0, 3*(display.getMaxCharHeight()-1));
    display.print(mc.humidity_adj, 1);
    display.print(F("%rel"));
    display.sendBuffer();

}

void MCDisplay::displayError(const std::string &who, const std::string &what)
{
    display.clearBuffer();
    // Print "who", will be cut at the end of the line if too long
    display.setFontMode(0);
    display.setFont(u8g2_font_tenfatguys_tf);
    display.drawStr(0,18,who.c_str());

    // Print "what", long messages will be scrolled horizontally.
    display.setFont(u8g2_font_inb16_mr); // this must be here to make the next line work
    u8g2_uint_t width = display.getUTF8Width(what.c_str()); // get the pixel width of the what string
    int16_t offset = 0; // we start from the rightmost pixel
    do {
        u8g2_uint_t x = offset;
        do {
            display.drawStr(x,50,what.c_str());
            x += width;
        }
        while(x < display.getDisplayWidth());
        
        display.sendBuffer();
        offset -= 5;
        if (offset < -(width))
            break;
        delay(1);
    }
    while (true);
}