#include "ntc.h"
#include <Arduino.h>

static int ntcPin;
static int SERIESRESISTOR;
static float NOMINAL_RESISTANCE;
static float NOMINAL_TEMPERATURE;
static float BCOEFFICIENT;

void NTC::init(int analogPin, int seriesResistor, float nominalRes, float nominalTemp, float bCoef) {
    ntcPin = analogPin;
    SERIESRESISTOR = seriesResistor;
    NOMINAL_RESISTANCE = nominalRes;
    NOMINAL_TEMPERATURE = nominalTemp;
    BCOEFFICIENT = bCoef;

    pinMode(ntcPin, INPUT);
}

float NTC::readTemperature() {
    int reading = analogRead(ntcPin);
    float voltage = reading * (3.3 / 4095.0);
    float resistance = SERIESRESISTOR / ((3.3 / voltage) - 1.0);

    float steinhart = resistance / NOMINAL_RESISTANCE;
    steinhart = log(steinhart);
    steinhart /= BCOEFFICIENT;
    steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
    steinhart = 1.0 / steinhart;
    steinhart -= 273.15;

    return steinhart;
}
