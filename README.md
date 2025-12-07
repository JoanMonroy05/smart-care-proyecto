# 🏥 SMART CARE - Sistema de Monitoreo de Signos Vitales

## 📋 Descripción

SMART CARE es un sistema de monitoreo de signos vitales que integra cuatro funcionalidades independientes: alcoholímetro, medidor de pulso cardíaco, termómetro corporal y oxímetro. 

Para evitar la sobrecarga de un solo microcontrolador, el proyecto utiliza **dos ESP32** que distribuyen el procesamiento de los sensores. Los datos se visualizan mediante conexión inalámbrica a dispositivos móviles.

## ✨ Funcionalidades

- **Alcoholímetro**: Sensor MQ-3
- **Frecuencia Cardíaca**: Sensor HW827
- **Temperatura Corporal**: Termistor NTC
- **Oxímetro**: MAX30102

## 🛠️ Hardware Utilizado

### Componentes Principales
- 2x ESP32 DevKit
- 1x Sensor MQ-3 (Alcoholímetro)
- 1x Sensor HW827 (Pulso cardíaco)
- 1x Termistor NTC 10kΩ
- 1x Sensor MAX30102 (Oximetría)

### Componentes Auxiliares
- 2x Protoboard 830 puntos
- Cables Dupont (Hembra-Macho y Macho-Macho)
- LEDs indicadores
- Resistencias varias (220Ω, 10kΩ)
- 2x Cable micro-USB

## 📁 Estructura del Proyecto

```
smart-care/
│
├── medical-care/              # ESP32 #1 - Desarrollado en VS Code + PlatformIO
│   └── src/
│       └── main.cpp          # MQ-3, NTC y MAX30102
│
├── HeardRate_AduinoIDE/       # ESP32 #2 - Desarrollado en Arduino IDE
│   └── HeardRate_AduinoIDE.ino  # HW827 (Sensor de pulso cardíaco)
│
└── README.md
```

## 🔧 Entornos de Desarrollo

El proyecto utiliza dos entornos de desarrollo diferentes:

**medical-care** → VS Code + PlatformIO
- Maneja 3 sensores: MQ-3, NTC y MAX30102
- Incluye servidor web para visualización de datos

**HeardRate_AduinoIDE** → Arduino IDE
- Maneja el sensor HW827 de pulso cardíaco

Esta división en dos ESP32 evita la sobrecarga del microcontrolador y garantiza lecturas estables de todos los sensores.

## 🛠️ Hardware Utilizado

- 2x ESP32 DevKit
- 1x Sensor MQ-3 (Alcoholímetro)
- 1x Sensor HW827 (Pulso cardíaco)
- 1x Termistor NTC 10kΩ
- 1x Sensor MAX30102 (Oximetría)
- 2x Protoboard
- Cables Dupont
- LEDs y resistencias
- 2x Cable micro-USB

## 🔌 Conexiones Básicas

Los sensores se conectan a los pines analógicos del ESP32 correspondiente. La alimentación se realiza desde la protoboard conectada a 3.3V y GND del ESP32.

---

**Proyecto desarrollado como práctica de laboratorio de Electrónica General**
