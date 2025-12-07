#include "sensor_manager.h"
#include <Arduino.h>
#include "mq3.h"
#include "ntc.h"
#include "max30102.h"

// Inicialización de variables estáticas
float SensorManager::mq3Value         = 0.0f;
float SensorManager::temperaturaValue = 0.0f;

float SensorManager::spo2Value        = 0.0f;
bool  SensorManager::spo2Valid        = false;


unsigned long SensorManager::lastSlowUpdate = 0;
unsigned long SensorManager::lastFastUpdate = 0;

void SensorManager::init() {
    MQ3::init(34, 26, 27);
    NTC::init(35, 10000, 10000, 25, 3950);
    MAXSensor::init();
    
    lastSlowUpdate = 0;
    lastFastUpdate = 0;
    
    Serial.println("SensorManager inicializado");
}

void SensorManager::update() {
    unsigned long now = millis();
    
    // Actualización lenta para otros sensores (cada 500ms)
    if (now - lastSlowUpdate >= 50) {
        lastSlowUpdate = now;

        // MQ3 - Sensor de alcohol
        mq3Value = MQ3::readAlcohol();
        MQ3::updateLeds(mq3Value);
        
        // NTC - Temperatura
        temperaturaValue = NTC::readTemperature();

        // MAX30102 - SpO2
        MAXSensor::update();
        if (MAXSensor::isReady()) {
            spo2Value = MAXSensor::spo2Value;
            spo2Valid = MAXSensor::spo2Valid;
        }
    }
}