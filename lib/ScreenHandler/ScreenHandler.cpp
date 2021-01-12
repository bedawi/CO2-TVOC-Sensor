#include "ScreenHandler.h"
#include "ScreenPage.h"

ScreenHandler::ScreenHandler()
{
    _firstScreen = NULL;
    _lastShownScreen = NULL;
    m_priority = 1; // only show screens with this priority
}

void ScreenHandler::attachScreen(ScreenPage *newScreen)
{
    if (this->_firstScreen == NULL)
    {
        // No screen has been attached so far. Attaching pointer to screen as first
        this->_firstScreen = newScreen;
        this->_lastShownScreen = newScreen;
    }
    else
    {
        // go to the list until the end
        ScreenPage *currentScreen = this->_firstScreen;
        while (currentScreen->_nextScreen != NULL)
        {
            currentScreen = currentScreen->_nextScreen;
        }
        // add a pointer to the new screen to the last screen in the list
        currentScreen->_nextScreen = newScreen;
    }
}

uint8_t ScreenHandler::getScreensCount(uint8_t priority)
{
    uint8_t count = 0;
    ScreenPage *currentScreen = this->_firstScreen;
    while (currentScreen->_nextScreen != NULL)
    {
        if (currentScreen->getPriority() == priority)
        {
            count++;
        }
        currentScreen = currentScreen->_nextScreen;
    }
    return count;
}

ScreenPage *ScreenHandler::returnNextScreen()
{
    // if any screen has a higher priority then 1, then change the priority of screens we present to 2.
    // screens with priority 1 (and always 0) will not be shown.
    if (this->getScreensCount(2) != 0)
    {
        m_priority = 2;
    }
    else
    {
        if (this->getScreensCount(1) != 0)
        {
            m_priority = 1;
        }
        else
        {
            // Fallback: If no screens with either priority 2 or 1 are defined, then...
            m_priority = 0;
        }
    }

    do
    {
        if (this->_lastShownScreen->_nextScreen == NULL)
        {
            this->_lastShownScreen = this->_firstScreen;
        }
        else
        {
            this->_lastShownScreen = this->_lastShownScreen->_nextScreen;
        }
    } while (this->_lastShownScreen->getPriority() != m_priority);

    return this->_lastShownScreen;
}
