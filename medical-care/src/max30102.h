#pragma once

#include <stdint.h>

class MAXSensor {
public:
    static void init();
    static void update();
    static bool isReady();

    static float spo2Value;
    static bool spo2Valid;
    static float heartRate;
    static bool heartRateValid;

private:
    static bool dataReady;
    static int sampleCount;
    static const int BUFFER_SIZE = 100;

public:
    static uint32_t irBuffer[BUFFER_SIZE];
    static uint32_t redBuffer[BUFFER_SIZE];
};
