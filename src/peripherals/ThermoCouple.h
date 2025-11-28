#ifndef _THERMOCOUPLE_H_
#define _THERMOCOUPLE_H_

#include <max6675.h>
#include "TemperatureSensor.h"

class ThermoCouple : TemperatureSensor {
    MAX6675* sensor;
public:
    /**
     * pin - used as chip select pin
     */
    ThermoCouple(const uint8_t pin);
    float getTemperature() override;
};

#endif