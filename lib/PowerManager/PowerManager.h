#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <Arduino.h>

class PowerManager {
  private:
    const uint8_t _vextPin = 36;
    const uint8_t _mosfetPin = 48;

  public:
    PowerManager();
    void begin();
    void sensorsOn();
    void sensorsOff();
    void gpsOn();
    void gpsOff();
    void sleep(uint64_t seconds);
};

#endif