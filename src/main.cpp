/**
 * @file main.cpp
 * @brief Main entry point for Forest Monitor Nodes telemetry device
 * 
 * This file contains the main program logic for the Forest Monitor Nodes device.
 * It initializes all subsystems, handles the initial setup process, and manages
 * the telemetry cycle including sensor readings, LoRa transmission, and power
 * management for deep sleep.
 * 
 * @author Joao Fialho nº 2301428
 * @version 1.0.0
 * @date 2026-04-11
 * 
 * @note The device operates in a cycle: initialize -> read sensors -> transmit
 *       via LoRa -> deep sleep -> wake up and repeat.
 * @note Initial setup mode is accessible via button press or serial command
 *       within the first 60 seconds of boot.
 * @note Battery level monitoring adjusts sleep duration automatically.
 */

#include <Arduino.h>
#include "PowerManager.h"
#include "SetupManager.h"
#include "SensorsManager.h"
#include "LoRaManager.h"
#include "GPSManager.h"
#include "../include/config.h"

RTC_DATA_ATTR bool initialConfigDone = false;

PowerManager power;
SetupManager setupMenu;
SensorsManager sensors;
LoRaManager lora;
GPSManager gps;

/**
 * @brief System initialization and setup routine
 * 
 * This function performs the following operations:
 * - Initializes serial communication for debugging
 * - Initializes all subsystems (Power, Sensors, LoRa)
 * - Handles initial configuration mode if not previously configured
 * - Executes the telemetry cycle: sensor reading, LoRa transmission
 * - Puts device into deep sleep based on battery level
 * 
 * The setup process includes:
 * 1. A 60-second window for initial configuration access
 * 2. Button press detection (2s hold) or serial command to enter setup
 * 3. Sensor stabilization period
 * 4. Telemetry data collection and transmission
 * 5. Adaptive sleep duration based on battery voltage
 */
void setup()
{
    Serial.begin(115200);
    power.begin();
    sensors.begin();
    lora.begin();

    if (!initialConfigDone)
    {
        APP_DEBUG_PRINTLN(">>> MODO SETUP INICIAL (60s) <<<");
        setupMenu.begin();
        power.gpsOn();
        power.sensorsOn();
        delay(2000);
        gps.begin();
        digitalWrite(LED_BUILTIN, HIGH);
        uint32_t setupStart = millis();
        bool inSetup = false;
        while (millis() - setupStart < 60000)
        {
            if (digitalRead(BUTTON_PIN) == LOW)
            { 
                uint32_t pressStart = millis();
      
                while (digitalRead(BUTTON_PIN) == LOW)
                {
                    if (millis() - pressStart > 2000)
                    { 
                        APP_DEBUG_PRINTLN("\n[EVENTO] Botao segurado por 2s!");
                        inSetup = true;
                    }
                    delay(10);
                    if (inSetup) break; 
                }
            }

            if (Serial.available() > 0)
            {
                String input = Serial.readStringUntil('\n');
                inSetup = true;
                APP_DEBUG_PRINTLN("\n[EVENTO] Comando recebido! Entrando no Setup..");
                break;
            }

            delay(50);
            if(inSetup){
                break;
            }
        }

        digitalWrite(LED_BUILTIN, LOW);
        initialConfigDone = true;

        if (inSetup)
        {
            setupMenu.run(sensors, lora, gps);
        }

        power.gpsOff();
        APP_DEBUG_PRINTLN("Setup Terminado.");
    }

    Serial.println("Executando telemetria..");
    power.sensorsOn();
    delay(1500);
    float temperature = sensors.getTemperature();
    float humidity = sensors.getHumidity();
    float batteryVoltage = sensors.getBatteryVoltage();
    float soilMoisture = sensors.getSoilMoisture();
    float windSpeed = sensors.getWindSpeed();
    APP_DEBUG_PRINTF("Temp: %.2f C, Hum: %.2f %, Bat: %.2f V\n", temperature, humidity, batteryVoltage);
    delay(1500);
    lora.sendTelemetry(temperature, humidity, batteryVoltage, soilMoisture, windSpeed);
    delay(1500);
    uint32_t currentSleep;
    if(batteryVoltage < BATTERY_LOW_THRESHOLD && batteryVoltage > 1.0) {
        APP_DEBUG_PRINTLN("Bateria baixa! Entrando em modo de baixo consumo..");
        currentSleep = SLEEP_TIME_LOW_BATTERY;
    } else {
        currentSleep = DEBUG_MODE ? SLEEP_TIME_DEBUG : SLEEP_TIME_PROD;
    }
    APP_DEBUG_PRINTLN("A dormir..");
    power.sleep(currentSleep);
}

/**
 * @brief Main loop function
 * 
 * This function remains empty as the device enters deep sleep immediately
 * after the setup() function completes. The ESP32 will wake up after
 * the configured sleep duration and restart from setup().
 * 
 * @note The device operates in a wake-process-sleep cycle.
 * @note All processing occurs in setup(), not in loop().
 */
void loop()
{
}