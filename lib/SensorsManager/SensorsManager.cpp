#include "SensorsManager.h"

DHT dht(DHTPIN, DHTTYPE);

void SensorsManager::begin() {
    pinMode(ADC_BAT_CTRL, OUTPUT);
    digitalWrite(ADC_BAT_CTRL, LOW);
    dht.begin();
}

float SensorsManager::getTemperature() {
    return dht.readTemperature(); // Retorna Celsius
}

float SensorsManager::getHumidity() {
    return dht.readHumidity();
}

float SensorsManager::getBatteryVoltage() {
    digitalWrite(ADC_BAT_CTRL, HIGH);
    delay(10);
    // 4.9 é um fator de calibração para o divisor de tensão e a referência do ADC
    float voltage = analogReadMilliVolts(ADC_BAT_PIN) * 4.9 / 1000.0;

    digitalWrite(ADC_BAT_CTRL, LOW);

    return voltage;
}

float SensorsManager::getSoilMoisture() {
    int rawValue = analogRead(SOIL_PIN);
    APP_DEBUG_PRINTLN("Raw Soil Moisture Value: " + String(rawValue));
    return rawValue;
}

float SensorsManager::getWindSpeed() {
    // TODO
    return 0.0; // Retorna um valor de velocidade do vento (m/s)
}
