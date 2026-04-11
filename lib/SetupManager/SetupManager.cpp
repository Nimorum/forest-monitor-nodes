/**
 * @file SetupManager.cpp
 * @brief Implementation of the SetupManager class.
 * @details Handles the initialization of the OLED display and the execution of the
 * initial setup sequence including GPS fix, sensor reading, and LoRa registration.
 */

#include "SetupManager.h"

/**
 * @brief Constructor for the SetupManager class.
 * @details Initializes the display driver with the predefined OLED parameters (I2C address and pins).
 * @return SetupManager instance.
 */
SetupManager::SetupManager() : 
    _exitMenu(false),
    display(0x3c, OLED_SDA, OLED_SCL, GEOMETRY_128_64) {}

/**
 * @brief Initializes the OLED display hardware.
 * @details Performs a hardware reset of the OLED, initializes the driver, sets orientation, 
 * and configures the default font.
 * @return void
 * @note Failure to initialize the display will be logged via DEBUG_PRINTLN.
 */
void SetupManager::begin() {

    pinMode(OLED_RST, OUTPUT);
    delay(10);
    digitalWrite(OLED_RST, HIGH);
    delay(10);
    digitalWrite(OLED_RST, LOW);
    delay(50);
    digitalWrite(OLED_RST, HIGH);
    delay(50);

    if (!display.init()) {
        DEBUG_PRINTLN("[ERRO] Falha ao inicializar o OLED!");
    }
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.clear();
}

/**
 * @brief Updates the OLED display with the current setup step and status.
 * @details Clears the screen and draws a standard header, followed by the specific step and status message.
 * @param step (String) The name of the current setup step being executed.
 * @param status (String) The current status or result of the step.
 * @return void
 * @note This method also outputs the information to the debug serial port in debug mode.
 */
void SetupManager::updateDisplay(String step, String status) {
    display.clear();
    
    display.drawString(0, 0, "--- SETUP MODE ---");
    display.drawString(0, 20, step);
    display.drawString(0, 40, status);
    
    display.display();
    
    DEBUG_PRINT(step);
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(status);
}

/**
 * @brief Runs the complete setup and registration sequence.
 * @details Executes a multi-step process:
 * 1. Waits for a valid GPS fix.
 * 2. Reads and verifies all connected sensors.
 * 3. Performs node registration through the LoRa network.
 * @param sensors (SensorsManager&) Reference to the sensors management object.
 * @param lora (LoRaManager&) Reference to the LoRa communication management object.
 * @param gps (GPSManager&) Reference to the GPS management object.
 * @return void
 * @note This is a blocking call that will only return once registration is successful.
 */
void SetupManager::run(SensorsManager& sensors, LoRaManager& lora, GPSManager& gps) {
    _exitMenu = false;
    float currentLat = 0.0;
    float currentLng = 0.0;
    bool hasFix = false;

    updateDisplay("1. GPS Status", "Searching...");

    // Loop until we get a valid fix
    uint32_t lastDebugPrint = 0;

    while (!hasFix) {
        hasFix = gps.getPosition(currentLat, currentLng);

        if (!hasFix) {
            if (millis() - lastDebugPrint > 2000) {
                gps.printDebug();
                lastDebugPrint = millis();
            }
            delay(10);
        }
    }

    updateDisplay("1. GPS Status", "FIX OK!");
    delay(1000);

    updateDisplay("2. Sensors", "Reading...");
    float temp = sensors.getTemperature();
    float hum = sensors.getHumidity();
    float bat = sensors.getBatteryVoltage();
    float soil = sensors.getSoilMoisture();
    float wind = sensors.getWindSpeed();

    DEBUG_PRINTF("Temp: %.2f C, Hum: %.2f %%, Bat: %.2f V, Soil: %.2f %%, Wind: %.2f m/s\n", temp, hum, bat, soil, wind);
    
    String sensorStatus = String(temp, 1) + "C  " + String(hum, 1) + "%";
    updateDisplay("2. Sensors", sensorStatus);

    delay(2000);
    updateDisplay("2. Sensors", "ALL OK");
    delay(1000);

    updateDisplay("3. LoRa Reg", "Sending...");
    bool registered = false;
    
    while (!registered) {
        registered = lora.sendRegistration(currentLat, currentLng);
        
        if (!registered) {
            updateDisplay("3. LoRa Reg", "Failed. Retrying...");
            delay(2000);
        }
    }
    
    updateDisplay("3. LoRa Reg", "SUCCESS!");
    delay(2000);

    display.clear();
    display.display();

    _exitMenu = true;
}