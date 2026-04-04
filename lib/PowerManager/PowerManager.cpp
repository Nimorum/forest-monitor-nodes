#include "PowerManager.h"

PowerManager::PowerManager() {}

void PowerManager::begin() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(V_EXT_PIN, OUTPUT);
    pinMode(GPS_VCC_PIN, OUTPUT);
    sensorsOff();
    gpsOff();
}

void PowerManager::sensorsOn() {
  digitalWrite(V_EXT_PIN, LOW); 
}

void PowerManager::sensorsOff() {
  digitalWrite(V_EXT_PIN, HIGH);
}

void PowerManager::gpsOn() {
  digitalWrite(GPS_VCC_PIN, LOW);
}

void PowerManager::gpsOff() {
  digitalWrite(GPS_VCC_PIN, HIGH);
}

void PowerManager::sleep(uint64_t seconds) {
  sensorsOff();
  gpsOff();
  esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
  esp_deep_sleep_start();
}