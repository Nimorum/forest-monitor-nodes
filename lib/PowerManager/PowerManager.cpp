#include "PowerManager.h"

/**
 * @brief Default constructor for PowerManager.
 *
 * No dynamic resources are allocated at construction time; all hardware
 * initialisation is deferred to begin().
 */
PowerManager::PowerManager() {}

/**
 * @brief Initialises GPIO pins and sets the system to a safe power-off state.
 *
 * Configures LED_BUILTIN, V_EXT_PIN, and GPS_VCC_PIN as outputs, then
 * immediately powers down the external sensor rail and the GPS module so
 * that no peripheral draws current before it is explicitly enabled.
 */
void PowerManager::begin() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(V_EXT_PIN, OUTPUT);
    pinMode(GPS_VCC_PIN, OUTPUT);
    sensorsOff();
    gpsOff();
}

/**
 * @brief Powers on the external sensor rail.
 *
 * Drives V_EXT_PIN LOW to enable the P-channel MOSFET (or equivalent switch)
 * that supplies voltage to the peripheral sensors.
 */
void PowerManager::sensorsOn() {
  digitalWrite(V_EXT_PIN, LOW);
}

/**
 * @brief Powers off the external sensor rail.
 *
 * Drives V_EXT_PIN HIGH to disable the P-channel MOSFET (or equivalent
 * switch), cutting power to all peripherals connected to the external rail.
 */
void PowerManager::sensorsOff() {
  digitalWrite(V_EXT_PIN, HIGH);
}

/**
 * @brief Powers on the GPS module.
 *
 * Drives GPS_VCC_PIN LOW to enable the switch that supplies VCC to the GPS
 * receiver.
 */
void PowerManager::gpsOn() {
  digitalWrite(GPS_VCC_PIN, LOW);
}

/**
 * @brief Powers off the GPS module.
 *
 * Drives GPS_VCC_PIN HIGH to cut power to the GPS receiver, reducing
 * quiescent current during idle periods.
 */
void PowerManager::gpsOff() {
  digitalWrite(GPS_VCC_PIN, HIGH);
}

/**
 * @brief Puts the ESP32 into deep sleep for the specified duration.
 *
 * Before entering deep sleep, all peripherals are powered down by calling
 * sensorsOff() and gpsOff(). A timer wake-up source is configured for the
 * requested interval, after which esp_deep_sleep_start() is called. The MCU
 * will not return from this function; execution resumes from the reset vector
 * after the sleep period expires.
 *
 * @param seconds Duration of the deep-sleep period in seconds.
 */
void PowerManager::sleep(uint64_t seconds) {
  sensorsOff();
  gpsOff();
  esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
  esp_deep_sleep_start();
}
