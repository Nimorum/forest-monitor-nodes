# Forest Monitor - Sensor Nodes (Firmware) 🔋🍃

Este repositório contém o firmware em C++ para os nós sensores autónomos baseados no microcontrolador ESP32 com LoRa integrado.

## 📌 Sobre o Hardware e Firmware
Os nós sensores foram desenhados para máxima eficiência energética em ambiente rural, recorrendo a estratégias de Deep Sleep e gestão de energia via hardware (MOSFET P-Channel).

**Ciclo de Operação:**
1. **Setup Mode (Interativo):** Ao ligar/registar, utiliza o ecrã OLED integrado para validação de GPS e sensores, enviando um pacote inicial com as coordenadas de instalação.
2. **Deep Sleep:** O nó desliga os periféricos (incluindo o GPS via MOSFET) para poupar energia.
3. **Telemetria (Cíclica):** A cada 10 minutos, o nó acorda, lê os sensores ambientais e transmite um payload LoRa contendo as variáveis microclimáticas e a voltagem da bateria, voltando a hibernar de imediato.

## 🛠️ Componentes Suportados
- **Microcontrolador:** Placa de desenvolvimento ESP32 LoRa V3 (868 MHz)
- **Corte de Energia:** MOSFET P-Channel (AO3401)
- **Sensores:**
  - Humidade e Temperatura do Solo
  - BME280 (Temperatura, Humidade do Ar, Pressão)
  - Anemómetro (Reed switch / interrupções de hardware)
  - Módulo GPS NEO-6M (apenas para deploy)

## 🚀 Compilação e Upload
Projeto estruturado para o **PlatformIO** (ou compatível com Arduino IDE).
1. Configurar o pinout no ficheiro `include/config.h`.
2. Compilar e fazer o upload via USB.
