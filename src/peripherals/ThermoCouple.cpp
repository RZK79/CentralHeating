#include "ThermoCouple.h"
#include <SPI.h>

ThermoCouple::ThermoCouple(const uint8_t pin){
    readPin = pin;
    sensor = new MAX6675(SCK, readPin, MISO);

    for (int i = 0; i < 10; i++) {
        buf[i] = sensor->readCelsius();
    }
}

float ThermoCouple::getTemperature()
{
    float sum = 0;
    for (int i = 0; i < 9; i++) {
        buf[i] = buf[i + 1];
        sum += buf[i];
    }

    buf[9] = sensor->readCelsius();
    sum += buf[9];

    return sum / 10.0f;
}