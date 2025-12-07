#pragma once

class SensorManager {
public:
    // Variables públicas de sensores
    static float spo2Value;
    static bool spo2Valid;
    static float mq3Value;
    static float temperaturaValue;

    // Métodos principales
    static void init();
    static void update();

private:
    static unsigned long lastSlowUpdate;
    static unsigned long lastFastUpdate;
};