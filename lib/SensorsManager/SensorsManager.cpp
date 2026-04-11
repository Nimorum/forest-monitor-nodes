/**
 * @file SensorsManager.cpp
 * @brief Implementation of the SensorsManager class for the forest monitor node.
 *
 * @details Provides sensor abstraction for an ESP32-based environmental monitoring
 * node. Wraps a DHT temperature/humidity sensor, an ADC-based battery voltage
 * reader with an active-HIGH enable switch, a capacitive soil moisture sensor
 * mapped to a 0–100 % range, and a stub wind-speed reader pending anemometer
 * integration.
 */
#include "SensorsManager.h"

DHT dht(DHTPIN, DHTTYPE);

/**
 * @brief Initialises all sensors and configures required GPIO pins.
 *
 * @details Sets ADC_BAT_CTRL as an output and drives it LOW so that the
 * voltage-divider circuit is disabled by default, minimising quiescent current.
 * Then calls dht.begin() to power up and configure the DHT temperature /
 * humidity sensor.
 */
void SensorsManager::begin() {
    pinMode(ADC_BAT_CTRL, OUTPUT);
    digitalWrite(ADC_BAT_CTRL, LOW);
    dht.begin();
}

/**
 * @brief Reads the ambient temperature from the DHT sensor.
 *
 * @return Temperature in degrees Celsius as reported by the DHT sensor.
 *         Returns NaN if the sensor read fails.
 */
float SensorsManager::getTemperature() {
    return dht.readTemperature(); // Retorna Celsius
}

/**
 * @brief Reads the relative humidity from the DHT sensor.
 *
 * @return Relative humidity as a percentage in the range [0, 100].
 *         Returns NaN if the sensor read fails.
 */
float SensorsManager::getHumidity() {
    return dht.readHumidity();
}

/**
 * @brief Measures the battery voltage via the on-board ADC.
 *
 * @details Drives ADC_BAT_CTRL HIGH to connect the resistor voltage divider to
 * the ADC input, then waits 10 ms for the signal to stabilise. The raw reading
 * from analogReadMilliVolts() is scaled by a calibration factor of 4.9 / 1000.0
 * that accounts for the divider ratio and the ESP32 ADC reference offset.
 * ADC_BAT_CTRL is driven LOW again immediately after sampling to minimise
 * quiescent current draw.
 *
 * @return Battery voltage in Volts.
 */
float SensorsManager::getBatteryVoltage() {
    digitalWrite(ADC_BAT_CTRL, HIGH);
    delay(10);
    // 4.9 é um fator de calibração para o divisor de tensão e a referência do ADC
    float voltage = analogReadMilliVolts(ADC_BAT_PIN) * 4.9 / 1000.0;

    digitalWrite(ADC_BAT_CTRL, LOW);

    return voltage;
}

/**
 * @brief Reads the soil moisture level from the capacitive soil sensor.
 *
 * @details Samples the raw ADC value on SOIL_PIN and maps it from the
 * calibrated dry/wet window [SOIL_AIR_VAL, SOIL_WATER_VAL] to a
 * [0, 100] percentage. The result is clamped to [0, 100] to handle
 * readings that fall outside the calibrated range.
 *
 * @return Soil moisture percentage in the range [0.0, 100.0], where
 *         0 % represents completely dry soil and 100 % represents
 *         fully saturated soil.
 */
float SensorsManager::getSoilMoisture() {
    int rawValue = analogRead(SOIL_PIN);
    DEBUG_PRINTLN("Raw Soil Moisture Value: " + String(rawValue));
    // Mapeia o valor bruto para uma porcentagem de umidade
    float moisturePercent = map(rawValue, SOIL_AIR_VAL, SOIL_WATER_VAL, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100); // Garante que fique entre 0 e 100
    return moisturePercent;
}

/**
 * @brief Reads the current wind speed.
 *
 * @todo Implement anemometer interface. This method currently returns a
 *       fixed placeholder value and performs no hardware interaction.
 *
 * @return Wind speed in metres per second (currently always 0.0).
 */
float SensorsManager::getWindSpeed() {
    // TODO
    return 0.0; // Retorna um valor de velocidade do vento (m/s)
}
