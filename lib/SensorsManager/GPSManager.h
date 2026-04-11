/**
 * @file GPSManager.h
 * @brief Header file for the GPSManager class.
 * @details Handles communication with the NEO-6M GPS module via serial,
 * parsing NMEA sentences to extract valid position data.
 * 
 * @author Joao Fialho nº 2301428
 * @version 1.0.0
 * @date 2026-04-11
 * 
 * @note This class abstracts the GPS functionality, providing methods to initialize and debug the GPS module, as well as retrieve the current position when a valid fix is available.
 */

#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include "../../include/config.h"

class GPSManager {
private:
    TinyGPSPlus _gps;
    HardwareSerial _gpsSerial;

public:
    /**
     * @brief Constructor for the GPSManager class.
     * @details Initializes the HardwareSerial instance on Serial 2 for GPS communication.
     * @return GPSManager instance.
     */
    GPSManager();

    /**
     * @brief Initializes the GPS module serial communication.
     * @details Configures the serial port with the default NEO-6M baud rate (9600)
     * and standard serial settings (8 data bits, no parity, 1 stop bit).
     * @return void
     * @note Uses the RX and TX pins defined in config.h for GPS communication.
     */
    void begin();

    /**
     * @brief Retrieves the current GPS position.
     * @details Parses incoming serial data from the GPS module and extracts latitude
     * and longitude if a valid, fresh fix is available.
     * @param lat (float&) Reference to store the latitude in degrees.
     * @param lng (float&) Reference to store the longitude in degrees.
     * @return true if a valid position was obtained, false otherwise.
     * @note A valid fix requires:
     *       - A valid location from the GPS parser
     *       - Location age less than 2000ms (2 seconds)
     */
    bool getPosition(float &lat, float &lng);

    /**
     * @brief Outputs diagnostic information about the GPS module status.
     * @details Prints the number of characters processed, number of satellites
     * in use, and the Horizontal Dilution of Precision (HDOP) to the debug serial port.
     * @return void
     * @note This method is intended for debugging purposes and outputs to DEBUG_PRINT macros.
     */
    void printDebug();
};

#endif