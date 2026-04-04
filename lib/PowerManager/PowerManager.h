#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <Arduino.h>
#include "../../include/config.h"

class PowerManager {
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