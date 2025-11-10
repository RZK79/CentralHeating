#include "ThermoCouple.h"
#include <SPI.h>

ThermoCouple::ThermoCouple(const uint8_t pin) : TemperatureSensor(pin) {
    sensor = new MAX6675(SCK, readPin, MISO);
}

float ThermoCouple::getTemperature() {
    return sensor->readCelsius();
}