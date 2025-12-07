#include "max30102.h"
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

float MAXSensor::spo2Value = 0.0f;
bool  MAXSensor::spo2Valid = false;

float MAXSensor::heartRate = 0.0f;
bool  MAXSensor::heartRateValid = false;

bool MAXSensor::dataReady = false;

int MAXSensor::sampleCount = 0;

uint32_t MAXSensor::irBuffer[BUFFER_SIZE];
uint32_t MAXSensor::redBuffer[BUFFER_SIZE];

void MAXSensor::init() {
    Wire.begin(21, 22);

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("❌ MAX30102 no encontrado");
        return;
    }

    particleSensor.setup();

    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeIR(0x0A);

    Serial.println("✅ MAX30102 inicializado");

    sampleCount = 0;
    dataReady = false;
}

void MAXSensor::update() {
    if (!particleSensor.check()) return;
    if (!particleSensor.available()) return;

    uint32_t ir  = particleSensor.getIR();
    uint32_t red = particleSensor.getRed();
    particleSensor.nextSample();

    irBuffer[sampleCount]  = ir;
    redBuffer[sampleCount] = red;

    sampleCount++;

    if (sampleCount < BUFFER_SIZE) return;

    // -------- PROCESAR --------
    int32_t spo2;
    int8_t spo2ValidLocal;
    int32_t hr;
    int8_t hrValidLocal;

    maxim_heart_rate_and_oxygen_saturation(
        irBuffer,
        BUFFER_SIZE,
        redBuffer,
        &spo2,
        &spo2ValidLocal,
        &hr,
        &hrValidLocal
    );

    spo2Value      = spo2;
    spo2Valid      = spo2ValidLocal != 0;
    heartRate      = hr;
    heartRateValid = hrValidLocal != 0;

    sampleCount = 0;
    dataReady = true;
}

bool MAXSensor::isReady() {
    if (dataReady) {
        dataReady = false;
        return true;
    }
    return false;
}
