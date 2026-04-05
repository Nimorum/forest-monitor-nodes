#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/// ==========================================
/// Pinos de Sensores
/// ==========================================
#define V_EXT_PIN 36   // Controle de energia para sensores
#define DHTPIN 47     
#define DHTTYPE DHT22

// ==========================================
// Pinos do Sistema (Heltec V3 Internos)
// ==========================================
#define OLED_SDA 17
#define OLED_SCL 18
#define OLED_RST 21

// ==========================================
// Pinos do LoRa (SX1262 Internos)
// ==========================================
#define LORA_NSS  8
#define LORA_DIO1 14
#define LORA_NRST 12
#define LORA_BUSY 13

// ==========================================
// Pinos do GPS (Externos)
// ==========================================
#define GPS_RX_PIN 45   // Ligado ao TX do NEO-6M
#define GPS_TX_PIN 46   // Ligado ao RX do NEO-6M
#define GPS_VCC_PIN 4   // Gate do MOSFET (Power Control)

// ==========================================
// Outros Periféricos
// ==========================================
#define BUTTON_PIN 0    // Botão "PRG" do Heltec ou botão externo
#define ADC_BAT_PIN 1   // Exemplo: Pino de leitura de bateria (se usado)

// ==========================================
// Configurações de Rede LoRa
// ==========================================
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


// ==========================================
// Configurações de tempo de Energia
// ==========================================
#define DEBUG_MODE true
#define SLEEP_TIME_PROD 600
#define SLEEP_TIME_DEBUG 20
#define SLEEP_TIME_LOW_BATTERY 89400 // 24h
#define BATTERY_LOW_THRESHOLD 3.2 // Voltagem abaixo da qual consideramos a bateria "baixa"

#endif