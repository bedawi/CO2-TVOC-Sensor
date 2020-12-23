#ifndef _SCREENPAGE_H
#define _SCREENPAGE_H

#include <Arduino.h>


class ScreenPage
{
private:
    String m_prefix;
    String m_suffix;
    String m_unit;
    uint16_t m_value;
    bool m_readingAvailable;

public:
/*

*/
    ScreenPage(String prefix, String suffix, String unit);
    
    void setValue(uint16_t value);
    
    String getLine1();
    
    String getLine2();
    
    String getLine3();
    
};

#endif