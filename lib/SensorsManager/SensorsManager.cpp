#include "SensorsManager.h"

void SensorsManager::begin() {
    // DHT and Analog pins initialization will go here
}

void SensorsManager::printDummyData() {
    Serial.println("Temp: 22.5C | Hum: 55% | Wind: 12km/h");
}