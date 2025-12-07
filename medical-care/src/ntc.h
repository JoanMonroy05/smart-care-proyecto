#ifndef NTC_H
#define NTC_H

namespace NTC {
    void init(int analogPin, int seriesResistor, float nominalRes, float nominalTemp, float bCoef);
    float readTemperature();
}

#endif
