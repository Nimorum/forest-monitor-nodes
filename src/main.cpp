#include <Arduino.h>
#include "PowerManager.h"

#define DEBUG_MODE true
#define SLEEP_TIME_PROD 600
#define SLEEP_TIME_DEBUG 20

// #include "DisplayManager.h" // Futura classe para o OLED

PowerManager power;
RTC_DATA_ATTR bool initialConfigDone = false;
static const uint8_t BUTTON_PIN = 0;

void setup()
{
    Serial.begin(115200);
    power.begin();

    if (!initialConfigDone)
    {
        Serial.println(">>> MODO SETUP INICIAL (60s) <<<");
        power.gpsOn();
        power.sensorsOn();
        digitalWrite(LED_BUILTIN, HIGH);
        uint32_t setupStart = millis();
        bool inSetup = false;
        while (millis() - setupStart < 60000)
        {

            if (digitalRead(0) == LOW)
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
                    if (inSetup)
                        break; 
                }
            }

            if (Serial.available() > 0)
            {
                String input = Serial.readStringUntil('\n');
                inSetup = true;
                Serial.println("\n[EVENTO] Comando recebido! Entrando no Setup...");
                break;
            }
            delay(100);
            if(inSetup){
                break;
            }
        }

        digitalWrite(LED_BUILTIN, LOW);
        initialConfigDone = true;

        if (inSetup)
        {
            // open Lcd Menu
            // display.showSetupScreen();
        }

        power.gpsOff();
        Serial.println("Setup Terminado.");
    }

    // --- Ciclo Normal de Operação ---
    Serial.println("Executando telemetria...");
    power.sensorsOn();
    delay(1500);
    //read sensors
    delay(1500);
    //send data
    delay(1500);
    power.sensorsOff();

    Serial.println("A dormir...");
    uint32_t currentSleep = DEBUG_MODE ? SLEEP_TIME_DEBUG : SLEEP_TIME_PROD;
    power.sleep(currentSleep);
}

void loop()
{
    // O loop fica vazio porque o dispositivo entra em deep sleep após o setup
}