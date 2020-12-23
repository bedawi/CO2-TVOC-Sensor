#include "ScreenPage.h"


  ScreenPage::ScreenPage(String prefix, String suffix, String unit)
  {
    m_prefix = prefix;
    m_suffix = suffix;
    m_unit = unit;
    m_value = 0;
    m_readingAvailable = false;
  }

  void ScreenPage::setValue(uint16_t value)
  {
    m_value = value;
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
    return m_unit;
  }
