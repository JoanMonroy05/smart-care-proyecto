#pragma once

class MQ3 {
public:
    static void init(int pin, int ledVerde, int ledRojo);
    static float readAlcohol();
    static void updateLeds(float value);

private:
    static int mqPin;
    static int ledVerdePin;
    static int ledRojoPin;
};
