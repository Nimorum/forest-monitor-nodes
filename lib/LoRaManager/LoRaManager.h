#ifndef LORAMANAGER_H
#define LORAMANAGER_H

#include <Arduino.h>
#include <RadioLib.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "../../include/config.h"

class LoRaManager {
private:
    String getMacAddress();

public:
    bool begin();
    bool sendRegistration(float latitude, float longitude);
    bool sendTelemetry(float temperature, float humidity,float vbat, float soilMoisture, float windSpeed);
    bool sendData(String data);
};

#endif