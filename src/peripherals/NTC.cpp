#include "NTC.h"

void NTC::init(int resistance, const uint8_t analogPin, uint16_t Bvalue)
{
    readPin = analogPin;
    RT0 = resistance;
    B = Bvalue;
    for (int i = 0; i < 10; i++) {
        buf[i] = readSensor();
    }
}

float NTC::readSensor()
{
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

float NTC::getTemperature()
{
    float sum = 0;
    for (int i = 0; i < 9; i++) {
        buf[i] = buf[i + 1];
        sum += buf[i];
    }
    buf[9] = readSensor();
    sum += buf[9];
    return sum / 10.0f;
}

int NTC::ToValue(int type)
{
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
