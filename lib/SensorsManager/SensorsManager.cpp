#include "SensorsManager.h"

DHT dht(DHTPIN, DHTTYPE);

void SensorsManager::begin() {
    dht.begin();
}

float SensorsManager::getTemperature() {
    return dht.readTemperature(); // Retorna Celsius
}

float SensorsManager::getHumidity() {
    return dht.readHumidity();
}

float SensorsManager::getBatteryVoltage() {
    digitalWrite(ADC_BAT_PIN, LOW);
    delay(10);
    // 4.9 is the hardware multiplier from the Heltec V3 internal 390k/100k voltage divider
    float voltage = analogReadMilliVolts(ADC_BAT_PIN) * 4.9 / 1000.0;
    
    digitalWrite(ADC_BAT_PIN, HIGH);
    
    return voltage;
}

float SensorsManager::getSoilMoisture() {
    // TODO
    return 0.0; // Retorna um valor de umidade do solo (0-100%)
}

float SensorsManager::getWindSpeed() {
    // TODO
    return 0.0; // Retorna um valor de velocidade do vento (m/s)
}