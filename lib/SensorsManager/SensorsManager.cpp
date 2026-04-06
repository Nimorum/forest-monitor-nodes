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
    // 4.9 is the hardware multiplier from the Heltec V3 internal 390k/100k voltage divider
    float voltage = analogReadMilliVolts(ADC_BAT_PIN) * 4.9 / 1000.0;
    
    digitalWrite(ADC_BAT_CTRL, LOW);
    
    return voltage;
}

float SensorsManager::getSoilMoisture() {
    int rawValue = analogRead(SOIL_PIN);
    DEBUG_PRINTLN("Raw Soil Moisture Value: " + String(rawValue));
    // Mapeia o valor bruto para uma porcentagem de umidade
    float moisturePercent = map(rawValue, SOIL_AIR_VAL, SOIL_WATER_VAL, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100); // Garante que fique entre 0 e 100
    return moisturePercent;
}

float SensorsManager::getWindSpeed() {
    // TODO
    return 0.0; // Retorna um valor de velocidade do vento (m/s)
}