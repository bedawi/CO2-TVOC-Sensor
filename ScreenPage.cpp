#include "ScreenPage.h"

ScreenPage::ScreenPage(String prefix, String suffix, String comment)
{
  m_prefix = prefix;
  m_suffix = suffix;
  m_comment = comment;
  m_value = 0;
  m_readingAvailable = false;
  _nextScreen = NULL;
  m_priority = 1; // 0: Never shown 1: Shown during normal operation 2: important, overriding 1
}

void ScreenPage::setValue(uint16_t value)
{
  m_value = value;
}

void ScreenPage::setComment(String comment)
{
  m_comment = comment;
}

void ScreenPage::setPriority(uint8_t priority)
{
  m_priority = priority;
}

uint8_t ScreenPage::getPriority()
{
  return m_priority;
}

String ScreenPage::getLine1()
{
  String returnString = "";
  returnString = returnString + m_prefix + ":";
  return returnString;
}

String ScreenPage::getLine2()
{
  String returnString = "";
  returnString = returnString + m_value;

  if (m_suffix != "")
  {
    returnString = returnString + " " + m_suffix;
  }
  return returnString;
}

String ScreenPage::getLine3()
{
  return m_comment;
}
