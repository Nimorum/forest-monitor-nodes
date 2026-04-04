#include <Arduino.h>
#include "PowerManager.h"
#include "SetupManager.h"
#include "SensorsManager.h"
#include "LoRaManager.h"
#include "GPSManager.h"
#include "../../include/config.h"

RTC_DATA_ATTR bool initialConfigDone = false;

PowerManager power;
SetupManager setupMenu;
SensorsManager sensors;
LoRaManager lora;
GPSManager gps;

void setup()
{
    Serial.begin(115200);
    power.begin();
    sensors.begin();
    lora.begin();

    if (!initialConfigDone)
    {
        Serial.println(">>> MODO SETUP INICIAL (60s) <<<");
        setupMenu.begin();
        power.gpsOn();
        power.sensorsOn();
        delay(2000); // Aguarda os sensores estabilizarem
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
                        Serial.println("\n[EVENTO] Botao segurado por 2s!");
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
                Serial.println("\n[EVENTO] Comando recebido! Entrando no Setup...");
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
        Serial.println("Setup Terminado.");
    }

    // --- Ciclo Normal de Operação ---
    Serial.println("Executando telemetria...");
    power.sensorsOn();
    delay(1500);
    float temperature = sensors.getTemperature();
    float humidity = sensors.getHumidity();
    float batteryVoltage = sensors.getBatteryVoltage();
    float soilMoisture = sensors.getSoilMoisture();
    float windSpeed = sensors.getWindSpeed();
    Serial.printf("Temp: %.2f C, Hum: %.2f %%, Bat: %.2f V\n", temperature, humidity, batteryVoltage);
    delay(1500);
    lora.sendTelemetry(temperature, humidity, batteryVoltage, soilMoisture, windSpeed);
    delay(1500);
    uint32_t currentSleep;
    if(batteryVoltage < BATTERY_LOW_THRESHOLD && batteryVoltage > 1.0) { // Limite inferior para evitar leituras erradas ou danos
        Serial.println("Bateria baixa! Entrando em modo de baixo consumo...");
        currentSleep = SLEEP_TIME_LOW_BATTERY;
    } else {
        currentSleep = DEBUG_MODE ? SLEEP_TIME_DEBUG : SLEEP_TIME_PROD;
    }
    Serial.println("A dormir...");
    power.sleep(currentSleep);
}

void loop()
{
    // O loop fica vazio porque o dispositivo entra em deep sleep após o setup
}