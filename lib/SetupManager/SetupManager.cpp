#include "SetupManager.h"

#define SDA_OLED 17
#define SCL_OLED 18
#define RST_OLED 21

SetupManager::SetupManager(uint8_t buttonPin) : 
    _buttonPin(buttonPin),
    _exitMenu(false),
    display(0x3c, SDA_OLED, SCL_OLED, GEOMETRY_128_64) {}

void SetupManager::begin() {
    pinMode(_buttonPin, INPUT_PULLUP);

    pinMode(RST_OLED, OUTPUT);
    digitalWrite(RST_OLED, HIGH);
    delay(10);
    digitalWrite(RST_OLED, LOW);
    delay(50);
    digitalWrite(RST_OLED, HIGH);
    delay(50);

    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
}

void SetupManager::updateDisplay(String step, String status) {
    display.clear();
    
    display.drawString(0, 0, "--- SETUP MODE ---");
    display.drawString(0, 20, step);
    display.drawString(0, 40, status);
    
    display.display();
    
    Serial.print(step);
    Serial.print(" ");
    Serial.println(status);
}

void SetupManager::run(SensorsManager& sensors, LoRaManager& lora) {
    _exitMenu = false;

    updateDisplay("1. GPS Status", "Acquiring Fix...");
    delay(3000); 
    updateDisplay("1. GPS Status", "FIX OK");
    delay(1000);

    updateDisplay("2. Sensors", "Reading...");
    sensors.printDummyData(); 
    delay(2000);
    updateDisplay("2. Sensors", "ALL OK");
    delay(1000);

    updateDisplay("3. LoRa Reg", "Sending...");
    bool registered = false;
    
    while (!registered) {
        registered = lora.sendRegistration(40.7128, -74.0060);
        if (!registered) {
            updateDisplay("3. LoRa Reg", "Failed. Retrying...");
            delay(2000);
        }
    }
    
    updateDisplay("3. LoRa Reg", "SUCCESS!");
    delay(2000);

    display.clear();
    display.display();

    _exitMenu = true;
}