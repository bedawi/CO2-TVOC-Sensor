#include "SensorTimer.h"

SensorTimer::SensorTimer()
{
    m_repeat_time = 120000;
    m_warmup_time = 60000;
    startover();
    m_readings = false;
    m_coldstart_time = 120000;
    m_coldstart_at = millis();
    m_isAvailable = true;
}

void SensorTimer::startover()
{
    m_init_time = millis();
    m_waking_up_since = 0;
}

bool SensorTimer::isReady()
{
    if (millis() >= (m_init_time + m_repeat_time))
    {
        if (millis() >= (m_waking_up_since + m_warmup_time))
        {
            return true;
        }
    }
    return false;
}

bool SensorTimer::isWarmingUpFromColdstart()
{
    if (millis() < (m_coldstart_at + m_coldstart_time - m_warmup_time))
    {
        // Sensor is not yet ready from cold start.
        return true;
    }
    return false;
}

int SensorTimer::getReadyInSeconds()
{
    int return_value((m_coldstart_at + m_coldstart_time - m_warmup_time - millis()) / 1000);
    return return_value;
}

bool SensorTimer::isTimetoWakeup()
{
    if (isWarmingUpFromColdstart())
    {
        return false;
    }
    if (m_waking_up_since != 0)
    {
        return false;
    }
    if (millis() >= (m_init_time + m_repeat_time - m_warmup_time))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SensorTimer::setRepeatTime(int seconds)
{
    m_repeat_time = seconds * 1000;
}

void SensorTimer::setWarmupTime(int seconds)
{
    m_warmup_time = seconds * 1000;
}

void SensorTimer::setColdstartTime(int seconds)
{
    m_coldstart_time = seconds * 1000;
}

void SensorTimer::skipWaiting()
{
    m_init_time = millis() - m_repeat_time + m_warmup_time; // Changes the init-time so first measurement starts earlier.
}

void SensorTimer::wakeUp()
{
    m_waking_up_since = millis();
}

void SensorTimer::readingsTaken()
{
    m_readings = true;
}

bool SensorTimer::readingsWaiting()
{
    return m_readings;
}

void SensorTimer::readingsReported()
{
    m_readings = false;
}

void SensorTimer::setAvailable(bool isAvailable)
{
    m_isAvailable = isAvailable;
}

bool SensorTimer::isAvailable()
{
    return m_isAvailable;
}