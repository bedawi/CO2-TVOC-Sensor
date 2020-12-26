#ifndef _SCREENHANDLER_H
#define _SCREENHANDLER_H

#include <Arduino.h>
#include "ScreenPage.h"

/* 
 * The ScreenHandler is used to manage the different screen pages, which have been defined as objects of 
 * the ScreenPage class. 
 * 
 * Screens must be attached like this:
 *   myScreenHandler.attachScreen(&screen1_co2);
 * Screens have priority. Screens with priority 2 will always be shown over screens with priority 1.
 * Screens with priority 0 will never been shown. 
 * 
 * Attention: Program will hang if only screens with priority 0 are defined. The default priority of a screen is 1
 * 
 */
class ScreenHandler
{
private:
    uint8_t m_priority;
    ScreenPage* _firstScreen = NULL;
    ScreenPage* _lastShownScreen = NULL;

public:
    ScreenHandler();
    void attachScreen(ScreenPage* newScreen);
    uint8_t getScreensCount(uint8_t priority);
    ScreenPage* returnNextScreen();

};

#endif