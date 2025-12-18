#include "NTC.h"

NTC::NTC(int resistance, const uint8_t analogPin, uint16_t Bvalue) : TemperatureSensor(analogPin) {
    RT0 = resistance;
    B = Bvalue;
}

float NTC::getTemperature() {
    int a = analogRead(readPin);

    float URt = a * Ucc / 1023.0;
    float UR = Ucc - URt;
    float IR = UR / R;
    float RT = URt / IR;

    float ln = log(RT / RT0);
    float X = ln / B;
    float Y = 1.0f / (25.0f + 273.15f);

    return (1.0f / (X + Y)) - 273.15f;
}

int NTC::ToValue(int type) {
    switch (type) {
        case NTC_5k:
            return 5000;

        default:
        case NTC_10k:
            return 10000;

        case NTC_15k:
            return 15000;
            
        case NTC_20k:
            return 20000;
    }
}
