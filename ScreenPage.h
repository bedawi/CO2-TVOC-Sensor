#ifndef _SCREENPAGE_H
#define _SCREENPAGE_H

#include <Arduino.h>

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
    String m_prefix;
    String m_suffix;
    String m_comment;
    uint16_t m_value;
    bool m_readingAvailable;
    uint8_t m_priority;

public:
    ScreenPage *_nextScreen = NULL;
    /*

    */
    ScreenPage(String prefix, String suffix, String unit);
    void setValue(uint16_t value);
    void setComment(String comment);
    void setPriority(uint8_t priority);
    uint8_t getPriority();
    String getLine1();
    String getLine2();
    String getLine3();
};

#endif