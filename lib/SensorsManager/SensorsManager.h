#ifndef SENSORSMANAGER_H
#define SENSORSMANAGER_H

#include <Arduino.h>
#include "../../include/config.h"
#include "DHT.h"

class SensorsManager {
public:
    void begin();
    float getTemperature();
    float getHumidity();
    float getBatteryVoltage();
    float getSoilMoisture();
    float getWindSpeed();
};

#endif