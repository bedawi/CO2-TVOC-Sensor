#include "ScreenPage.h"

ScreenPage::ScreenPage(String prefix, String suffix, String comment):m_bitmap(nullptr)
{
  m_headline = prefix;
  m_suffix = suffix;
  m_comment = comment;
  m_value = 0;
  m_readingAvailable = false;
  _nextScreen = NULL;
  m_priority = 1;   // 0: Never shown 1: Shown during normal operation 2: important, overriding 1
  m_screenType = 0; // This is a ValueScreen - a Screen showing a value.
}

ScreenPage::ScreenPage(String headline, String infoMessage):m_bitmap(nullptr)
{
  m_headline = headline;
  m_infoMessage = infoMessage;
  m_screenType = 1; // This is an infoScreen - it does not show a value but a message like "WiFi Down"
  m_priority = 1;
  m_comment = "";
}

void ScreenPage::setValue(uint16_t value)
{
  m_value = value;
}

void ScreenPage::setInfomessage(String infomessage)
{
  m_infoMessage = infomessage;
}

void ScreenPage::setComment(String comment)
{
  m_comment = comment;
}

void ScreenPage::setPriority(uint8_t priority)
{
  m_priority = priority;
}

void ScreenPage::setIcon(uint16_t width, uint16_t height, unsigned char* icon)
{
  this->m_bitmap = icon;
  m_bitmap_width = width;
  m_bitmap_heigth = height;
}

uint8_t ScreenPage::getPriority()
{
  return m_priority;
}

String ScreenPage::getLine1()
{
  if (m_screenType == 0)
  {
    String returnString = "";
    returnString = returnString + m_headline + ":";
    return returnString;
  }
  else
  {
    return m_headline;
  }
}

String ScreenPage::getLine2()
{
  if (m_screenType == 0)
  {
    String returnString = "";
    returnString = returnString + m_value;

    if (m_suffix != "")
    {
      returnString = returnString + " " + m_suffix;
    }
    return returnString;
  }
  else
  {
    return m_infoMessage;
  }
}

String ScreenPage::getLine3()
{
  return m_comment;
}

unsigned char * ScreenPage::getIcon()
{
  return this->m_bitmap;
}

std::tuple<int, int> ScreenPage::getIconSize() {
  return std::make_tuple(m_bitmap_width, m_bitmap_heigth);
}