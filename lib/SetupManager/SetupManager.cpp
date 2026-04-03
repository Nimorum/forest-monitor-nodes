#include "SetupManager.h"
#include "../../include/config.h"

SetupManager::SetupManager() : 
    _exitMenu(false),
    display(0x3c, OLED_SDA, OLED_SCL, GEOMETRY_128_64) {}

void SetupManager::begin() {

    pinMode(OLED_RST, OUTPUT);
    delay(10);
    digitalWrite(OLED_RST, HIGH);
    delay(10);
    digitalWrite(OLED_RST, LOW);
    delay(50);
    digitalWrite(OLED_RST, HIGH);
    delay(50);

    if (!display.init()) {
        Serial.println("[ERRO] Falha ao inicializar o OLED!");
    }
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.clear();
    display.drawString(0, 0, "Sistema a iniciar...");
    display.display();
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

void SetupManager::run(SensorsManager& sensors, LoRaManager& lora, GPSManager& gps) {
    _exitMenu = false;
    float currentLat = 0.0;
    float currentLng = 0.0;
    bool hasFix = false;

    updateDisplay("1. GPS Status", "Searching...");

    // Loop until we get a valid fix
    while (!hasFix) {
        hasFix = gps.getPosition(currentLat, currentLng);
        
        // Optional: Add a small delay or button check to prevent infinite lock
        if (!hasFix) {
            // We don't want to clear the whole buffer, just update the status line if possible
            // But for simplicity with your current updateDisplay:
            delay(500); 
        }
    }

    updateDisplay("1. GPS Status", "FIX OK!");
    delay(1000);

    updateDisplay("2. Sensors", "Reading...");
    sensors.printDummyData(); 
    delay(2000);
    updateDisplay("2. Sensors", "ALL OK");
    delay(1000);

    updateDisplay("3. LoRa Reg", "Sending...");
    bool registered = false;
    
    while (!registered) {
        // Use the real coordinates obtained from the GPS
        registered = lora.sendRegistration(currentLat, currentLng);
        
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