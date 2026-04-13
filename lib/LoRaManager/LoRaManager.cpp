#include "LoRaManager.h"

SX1262 radio = new Module(LORA_NSS, LORA_DIO1, LORA_NRST, LORA_BUSY);

String LoRaManager::getMacAddress() {
    return WiFi.macAddress();
}

bool LoRaManager::begin() {
    int state = radio.begin(
            LORA_FREQ, 
            LORA_BW, 
            LORA_SF, 
            LORA_CR, 
            LORA_SYNC_WORD, 
            LORA_POWER, 
            LORA_PREAMBLE, 
            LORA_TCXO_VOLTAGE, 
            LORA_USE_LDO
        );
    
    if (state != RADIOLIB_ERR_NONE) {
        APP_DEBUG_PRINT("LoRa init failed, code: ");
        APP_DEBUG_PRINTLN(state);
        return false;
    }
    return true;
}

bool LoRaManager::sendRegistration(float latitude, float longitude) {
    JsonDocument doc;
    
    doc["mac_address"] = getMacAddress();
    doc["method"] = "register";
    
    if (latitude != 0.0 && longitude != 0.0) {
        doc["latitude"] = latitude;
        doc["longitude"] = longitude;
    } else {
        doc["latitude"] = nullptr;
        doc["longitude"] = nullptr;
    }

    String payload;
    serializeJson(doc, payload);

    APP_DEBUG_PRINT("TX: ");
    APP_DEBUG_PRINTLN(payload);

    return sendData(payload);
}

bool LoRaManager::sendTelemetry(float temperature, float humidity,float vbat, float soilMoisture, float windSpeed) {
    JsonDocument doc;
    
    doc["mac_address"] = getMacAddress();
    doc["method"] = "telemetry";
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["battery_voltage"] = vbat;
    doc["wind_speed"] = windSpeed;
    doc["soil_moisture"] = soilMoisture;

    String payload;
    serializeJson(doc, payload);

    APP_DEBUG_PRINT("TX: ");
    APP_DEBUG_PRINTLN(payload);

    return sendData(payload);
}

bool LoRaManager::sendData(String data) {
    String msg = data + "\r\n";
    int msgLen = msg.length();
    byte packet[msgLen + 4];

    // Waveshare DTU fixed transmission header: Target Addr High, Target Addr Low, Target Channel, and internal streaming byte
    packet[0] = 0x00; 
    packet[1] = 0x00; 
    packet[2] = 0x12; 
    packet[3] = 0x11; 

    memcpy(&packet[4], msg.c_str(), msgLen);

    int state = radio.transmit(packet, msgLen + 4);
    
    return (state == RADIOLIB_ERR_NONE);
}