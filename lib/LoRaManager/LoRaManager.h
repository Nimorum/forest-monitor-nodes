/**
 * @file LoRaManager.h
 * @brief Header file for the LoRaManager class for the forest monitor node.
 *
 * @details Provides LoRa communication functionality for the ESP32-based
 * environmental monitoring node. This module handles LoRa initialization,
 * node registration with the network, and telemetry data transmission.
 * It uses the SX1262 radio module with RadioLib library and integrates
 * with a Waveshare DTU for packet transmission.
 *
 * @author Joao Fialho nº 2301428
 * @version 1.0.0
 * @date 2026-04-11
 *
 * @note This module requires the SX1262 radio module to be connected
 *       with the pin definitions specified in config.h (LORA_NSS, LORA_DIO1,
 *       LORA_NRST, LORA_BUSY).
 * @note The LoRa configuration parameters (frequency, bandwidth, SF, CR,
 *       sync word, power, preamble) must match the network configuration.
 * @note Packet transmission uses a custom DTU header format with target
 *       address and channel bytes.
 */

#ifndef LORAMANAGER_H
#define LORAMANAGER_H

#include <Arduino.h>
#include <RadioLib.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "../../include/config.h"

/**
 * @class LoRaManager
 * @brief Manages LoRa communication for the forest monitor node.
 * @details The LoRaManager class provides comprehensive LoRa communication
 * functionality including initialization, node registration, and telemetry
 * data transmission. It wraps the SX1262 radio module from RadioLib and
 * handles packet formatting for the Waveshare DTU.
 *
 * The module supports two primary operations:
 * - Node registration: Sends device MAC address and GPS coordinates to
 *   register the node with the LoRa network.
 * - Telemetry transmission: Sends environmental sensor data including
 *   temperature, humidity, battery voltage, soil moisture, and wind speed.
 *
 * All data is serialized to JSON format before transmission.
 */
class LoRaManager {
private:
    /**
     * @brief Retrieves the MAC address from the WiFi module.
     * @details Uses the ESP32's built-in WiFi module to obtain the unique
     * MAC address of the device. This address is used to identify the node
     * in the LoRa network.
     * @return String containing the MAC address in standard format (e.g., "XX:XX:XX:XX:XX:XX").
     */
    String getMacAddress();

public:
    /**
     * @brief Constructs a new LoRaManager instance.
     * @details Initializes the LoRaManager object. No hardware initialization
     * occurs at this stage; the begin() method must be called to initialize
     * the LoRa radio module.
     */
    LoRaManager() {}

    /**
     * @brief Initializes the LoRa radio module.
     * @details Configures and initializes the SX1262 LoRa radio module with
     * the parameters defined in config.h (frequency, bandwidth, spreading
     * factor, coding rate, sync word, power, preamble, TCXO voltage, and
     * LDO usage).
     *
     * @return true if initialization succeeds, false otherwise.
     * @note This method should be called during the setup phase before
     *       any LoRa communication.
     * @note Failure will be indicated via APP_DEBUG_PRINT and APP_DEBUG_PRINTLN macros.
     */
    bool begin();

    /**
     * @brief Sends a node registration message to the LoRa network.
     * @details Creates and transmits a JSON payload containing the device's
     * MAC address and GPS coordinates for network registration. The payload
     * includes a "register" method indicator.
     *
     * @param latitude The GPS latitude coordinate. If 0.0, the field will be
     *        set to null in the JSON payload.
     * @param longitude The GPS longitude coordinate. If 0.0, the field will be
     *        set to null in the JSON payload.
     * @return true if transmission succeeds, false otherwise.
     * @note This method should be called after acquiring a valid GPS fix.
     * @note The MAC address is automatically retrieved from the WiFi module.
     */
    bool sendRegistration(float latitude, float longitude);

    /**
     * @brief Sends telemetry data to the LoRa network.
     * @details Creates and transmits a JSON payload containing environmental
     * sensor readings including temperature, humidity, battery voltage,
     * soil moisture, and wind speed. The payload includes a "telemetry"
     * method indicator.
     *
     * @param temperature Temperature reading in degrees Celsius.
     * @param humidity Relative humidity percentage in the range [0, 100].
     * @param vbat Battery voltage in Volts.
     * @param soilMoisture Soil moisture percentage in the range [0, 100].
     * @param windSpeed Wind speed in metres per second.
     * @return true if transmission succeeds, false otherwise.
     * @note This method should be called after reading all sensor data.
     * @note The MAC address is automatically retrieved from the WiFi module.
     */
    bool sendTelemetry(float temperature, float humidity, float vbat, float soilMoisture, float windSpeed);

    /**
     * @brief Sends arbitrary data payload via LoRa.
     * @details Formats the provided data string with a newline terminator,
     * wraps it in a DTU transmission header, and transmits via the LoRa radio.
     * The DTU header includes target address bytes (0x00, 0x00), target
     * channel (0x12), and streaming byte (0x11).
     *
     * @param data The JSON string or other data payload to transmit.
     * @return true if transmission succeeds, false otherwise.
     * @note This is a lower-level method used by sendRegistration and
     *       sendTelemetry.
     * @note The payload is automatically appended with a newline character.
     */
    bool sendData(String data);
};

#endif