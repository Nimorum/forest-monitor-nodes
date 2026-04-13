/**
 * @file SetupManager.h
 * @brief Header file for the SetupManager class.
 * @details This file defines the SetupManager class, which handles the initial configuration
 * and hardware verification sequence for the forest monitor node, including OLED display interaction.
 *
 * @author Joao Fialho nº 2301428
 * @version 1.0.0
 * @date 2026-04-11
 */

#ifndef SETUP_MANAGER_H
#define SETUP_MANAGER_H

#include <Arduino.h>
#include "Wire.h"
#include "SSD1306Wire.h"
#include "SensorsManager.h"
#include "LoRaManager.h"
#include "GPSManager.h"
#include "../../include/config.h"

/**
 * @class SetupManager
 * @brief Manages the initial setup and diagnostic routine.
 * @details The SetupManager class provides a structured way to initialize the hardware,
 * verify GPS connectivity, read initial sensor data, and register the node via LoRa.
 *
 * The setup sequence includes:
 * - OLED display initialization and verification
 * - GPS module power-up and position acquisition
 * - Sensor data validation
 * - LoRa network registration
 *
 * This class is designed to run once during device initialization to ensure
 * all hardware components are functioning correctly before normal operation begins.
 */
class SetupManager {
private:
    /**
     * @brief Flag to indicate if the setup menu has completed.
     */
    bool _exitMenu;

    /**
     * @brief Instance of the SSD1306 OLED display driver.
     */
    SSD1306Wire display;

    /**
     * @brief Updates the OLED display with the current setup step and status.
     * @details Clears the screen and draws a standard header, followed by the specific step and status message.
     * @param step The name of the current setup step being executed.
     * @param status The current status or result of the step.
     * @note This method also outputs the information to the debug serial port.
     */
    void updateDisplay(String step, String status);

public:
    /**
     * @brief Constructor for the SetupManager class.
     * @details Initializes the display driver with the predefined OLED parameters (I2C address and pins).
     */
    SetupManager();

    /**
     * @brief Initializes the OLED display hardware.
     * @details Performs a hardware reset of the OLED, initializes the driver, sets orientation,
     * and configures the default font.
     * @note Failure to initialize the display will be logged via APP_DEBUG_PRINTLN.
     */
    void begin();

    /**
     * @brief Runs the complete setup and registration sequence.
     * @details Executes a multi-step process:
     * 1. Waits for a valid GPS fix.
     * 2. Reads and verifies all connected sensors.
     * 3. Performs node registration through the LoRa network.
     * @param sensors Reference to the SensorsManager object for sensor readings.
     * @param lora Reference to the LoRaManager object for network registration.
     * @param gps Reference to the GPSManager object for position acquisition.
     * @note This is a blocking call that will not return until a GPS fix is acquired
     * and LoRa registration succeeds.
     */
    void run(SensorsManager& sensors, LoRaManager& lora, GPSManager& gps);
};

#endif