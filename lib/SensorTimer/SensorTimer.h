#ifndef _SENSORTIMER_H
#define _SENSORTIMER_H

#include <Arduino.h>

class SensorTimer
{
private:
  int m_repeat_time;
  int m_warmup_time;
  int m_coldstart_time;
  uint32_t m_init_time;
  uint32_t m_waking_up_since;
  uint32_t m_coldstart_at;
  bool m_readings;
  bool m_isAvailable;
  bool m_debugMode;

public:
  SensorTimer();

  void debug(bool onoff);

  void startover();

  bool isReady();

  bool isWarmingUpFromColdstart();

  int getReadyInSeconds();

  bool isTimetoWakeup();

  void setRepeatTime(int seconds);

  void setWarmupTime(int seconds);

  void setColdstartTime(int seconds);

  void skipWaiting();

  void wakeUp();

  void readingsTaken();

  bool readingsWaiting();

  void readingsReported();

  void setAvailable(bool isAvailable);

  bool isAvailable();

};

#endif