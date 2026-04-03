#ifndef SETUP_MANAGER_H
#define SETUP_MANAGER_H

#include <Arduino.h>
#include "Wire.h"
#include "SSD1306Wire.h"
#include "SensorsManager.h" 
#include "LoRaManager.h"    

class SetupManager {
private:
    uint8_t _buttonPin;
    bool _exitMenu;
    SSD1306Wire display;

    void updateDisplay(String step, String status);

public:
    SetupManager(uint8_t buttonPin);
    void begin();
    void run(SensorsManager& sensors, LoRaManager& lora);
};

#endif