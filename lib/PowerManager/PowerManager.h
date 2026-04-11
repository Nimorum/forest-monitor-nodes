/**
 * @file PowerManager.h
 * @brief Power Management Module for Forest Monitor Nodes
 * 
 * This module provides comprehensive power management functionality for the
 * Forest Monitor Nodes device. It controls power states for sensors, GPS,
 * and manages deep sleep modes to optimize battery life.
 * 
 * @author Joao Fialho nº 2301428
 * @version 1.0.0
 * @date 2026-04-11
 * 
 * @note This module is critical for extending battery life by controlling
 *       power states of various components and managing sleep cycles.
 * @note All power state changes should be performed through this module
 *       to ensure proper sequencing and timing.
 */

#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <Arduino.h>
#include "../../include/config.h"

/**
 * @class PowerManager
 * @brief Manages power states for Forest Monitor Nodes device
 * 
 * This class provides centralized control over power management for the
 * Forest Monitor Nodes device. It handles power states for sensors, GPS,
 * and manages deep sleep modes to optimize battery life.
 * 
 * @note The PowerManager is a singleton-like component that should be
 *       initialized early in the setup process.
 * @note All power state changes should be performed through this module
 *       to ensure proper sequencing and timing.
 */
class PowerManager {
  public:
    /**
     * @brief Constructs a new PowerManager instance
     * 
     * Initializes the PowerManager object with default values.
     * No hardware initialization occurs at this stage.
     */
    PowerManager();

    /**
     * @brief Initializes the PowerManager and prepares power control pins
     * 
     * Sets up the power control pins and initializes the power management
     * system. This should be called during the setup phase.
     */
    void begin();

    /**
     * @brief Powers on the sensor modules
     * 
     * Activates power to the sensor modules (DHT22, soil moisture, wind sensor).
     * Allows time for sensors to stabilize after power-up.
     * 
     * @note Should be called before reading sensor data.
     * @note Delays approximately 1500ms for sensor stabilization.
     */
    void sensorsOn();

    /**
     * @brief Powers off the sensor modules
     * 
     * Deactivates power to the sensor modules to conserve energy.
     * Should be called after sensor readings are complete.
     * 
     * @note Always call this after completing sensor operations.
     */
    void sensorsOff();

    /**
     * @brief Powers on the GPS module
     * 
     * Activates power to the GPS module for positioning operations.
     * Should be called before initializing or using the GPS.
     * 
     * @note Should be called before GPS initialization.
     * @note Allows time for GPS module to initialize.
     */
    void gpsOn();

    /**
     * @brief Powers off the GPS module
     * 
     * Deactivates power to the GPS module to conserve energy.
     * Should be called after GPS operations are complete.
     * 
     * @note Always call this after completing GPS operations.
     */
    void gpsOff();

    /**
     * @brief Puts the device into deep sleep mode
     * 
     * Puts the ESP32 device into deep sleep mode for the specified duration.
     * This is the primary method for conserving battery life between
     * telemetry cycles.
     * 
     * @param seconds Number of seconds to sleep (uint64_t for extended sleep times)
     * 
     * @note After sleep, the device will wake up and restart from setup().
     * @note This is the primary method for extending battery life.
     * @note Sleep duration can be adjusted based on battery level.
     */
    void sleep(uint64_t seconds);
};

#endif
