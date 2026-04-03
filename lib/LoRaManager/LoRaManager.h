#ifndef LORAMANAGER_H
#define LORAMANAGER_H

#include <Arduino.h>
#include <RadioLib.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define LORA_NSS 8
#define LORA_DIO1 14
#define LORA_NRST 12
#define LORA_BUSY 13

#define LORA_FREQ 868.0
#define LORA_BW 125.0
#define LORA_SF 9
#define LORA_CR 7
// 18 (0x12) is the standard sync word for private LoRa networks. LoRaWAN uses 52 (0x34).
#define LORA_SYNC_WORD 18 
#define LORA_POWER 10
#define LORA_PREAMBLE 8
#define LORA_TCXO_VOLTAGE 1.6
// Heltec V3 uses a DC-DC converter for the SX1262, so LDO is set to false for better efficiency.
#define LORA_USE_LDO false 

class LoRaManager {
private:
    String getMacAddress();

public:
    bool begin();
    bool sendRegistration(float latitude, float longitude);
    bool sendTelemetry(float temperature, float humidity,float pressure, float soilMoisture, float windSpeed);
    bool sendData(String data);
};

#endif