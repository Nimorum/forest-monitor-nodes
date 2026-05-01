#include "SensorsManager.h"

DHT dht(DHTPIN, DHTTYPE);

void SensorsManager::begin() {
    pinMode(ADC_BAT_CTRL, OUTPUT);
    pinMode(WIND_PIN, INPUT);
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
    long sum = 0;
    const int samples = 32;
    for(int i = 0; i < samples; i++) {
        sum += analogReadMilliVolts(WIND_PIN);
        delay(1); 
    }
    float vRead = (sum / (float)samples) / 1000.0;
    APP_DEBUG_PRINTLN("Average Wind Voltage: " + String(vRead) + " V");
    float vSource = vRead * WIND_DIVIDER_RATIO;
    APP_DEBUG_PRINTLN("Reconstructed Wind Voltage: " + String(vSource) + " V");
    if (vSource < WIND_MIN_VOLTAGE) {
        return 0.0;
    }
    float speedKmH = vSource * WIND_FACTOR;
    APP_DEBUG_PRINTLN("Calculated Wind Speed: " + String(speedKmH) + " km/h");
    return speedKmH;
}
