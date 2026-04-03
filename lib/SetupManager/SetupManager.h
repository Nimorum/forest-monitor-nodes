#ifndef SETUP_MANAGER_H
#define SETUP_MANAGER_H

#include <Arduino.h>
#include "Wire.h"
#include "SSD1306Wire.h"
#include "SensorsManager.h" 
#include "LoRaManager.h"
#include "GPSManager.h"

class SetupManager {
private:
    bool _exitMenu;
    SSD1306Wire display;

    void updateDisplay(String step, String status);

public:
    SetupManager();
    void begin();
    void run(SensorsManager& sensors, LoRaManager& lora, GPSManager& gps);
};

#endif