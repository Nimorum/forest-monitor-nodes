#include "PowerManager.h"

PowerManager::PowerManager() {}

void PowerManager::begin() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(_vextPin, OUTPUT);
    pinMode(_mosfetPin, OUTPUT);
    sensorsOff();
    gpsOff();
}

void PowerManager::sensorsOn() {
  digitalWrite(_vextPin, LOW); 
}

void PowerManager::sensorsOff() {
  digitalWrite(_vextPin, HIGH);
}

void PowerManager::gpsOn() {
  digitalWrite(_mosfetPin, LOW);
}

void PowerManager::gpsOff() {
  digitalWrite(_mosfetPin, HIGH);
}

void PowerManager::sleep(uint64_t seconds) {
  sensorsOff();
  gpsOff();
  esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
  esp_deep_sleep_start();
}