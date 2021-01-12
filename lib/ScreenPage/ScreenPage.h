#ifndef _SCREENPAGE_H
#define _SCREENPAGE_H

#include <Arduino.h>
#include <tuple>

/* 
 * The Screens class is used to define the different screen pages as objects. Screen objects contain the content 
 * that is to be shown on an oled screen.
 * 
 * Screens must be attached to a handler like this:
 *   myScreenHandler.attachScreen(&screen1_co2);
 */

class ScreenPage
{
private:
    String m_headline;
    String m_suffix;
    String m_comment;
    uint32_t m_value;
    bool m_readingAvailable;
    uint8_t m_priority;
    uint8_t m_screenType; // 0 = ValueScreen; 1 = Infoscreen;
    String m_infoMessage;
    unsigned char *m_bitmap;
    uint16_t m_bitmap_width;
    uint16_t m_bitmap_heigth;

public:
    ScreenPage *_nextScreen = NULL;
    /*

    */
    ScreenPage(String prefix, String suffix, String unit);
    ScreenPage(String headline, String infomessage);
    ScreenPage(uint16_t width, uint16_t height, unsigned char *icon);
    void setValue(uint32_t value);
    void setInfomessage(String infomessage);
    void setComment(String comment);
    void setPriority(uint8_t priority);
    void setIcon(uint16_t width, uint16_t height, unsigned char *icon);
    uint8_t getPriority();
    uint8_t getType();
    String getLine1();
    String getLine2();
    String getLine3();
    unsigned char *getIcon();
    std::tuple<int, int> getIconSize();
};

#endif