#include "mq3.h"
#include <Arduino.h>

int MQ3::mqPin;
int MQ3::ledVerdePin;
int MQ3::ledRojoPin;

float umbralAlcohol = 0.25;  // Ajusta según calibración real

void MQ3::init(int pin, int verde, int rojo) {
    mqPin = pin;
    ledVerdePin = verde;
    ledRojoPin = rojo;

    pinMode(mqPin, INPUT);
    pinMode(ledVerdePin, OUTPUT);
    pinMode(ledRojoPin, OUTPUT);

    digitalWrite(ledVerdePin, LOW);
    digitalWrite(ledRojoPin, LOW);
}

float MQ3::readAlcohol() {
    int raw = analogRead(mqPin);

    float volts = raw * (3.3 / 4095.0);
    float alcohol = volts * 0.8;  // Ajustar con calibración

    return alcohol;
}

void MQ3::updateLeds(float value) {
    if (value < umbralAlcohol) {
        // No alcohol
        digitalWrite(ledVerdePin, HIGH);
        digitalWrite(ledRojoPin, LOW);
    } else {
        // Sí alcohol
        digitalWrite(ledVerdePin, LOW);
        digitalWrite(ledRojoPin, HIGH);
    }
}
