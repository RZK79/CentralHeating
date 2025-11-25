#ifndef _TEMPERATURESENSOR_H_
#define _TEMPERATURESENSOR_H_

#include <Arduino.h>

class TemperatureSensor {
protected:
    uint8_t readPin;
public:
    TemperatureSensor(const uint8_t pin) {
        readPin = pin;
    }

    virtual float getTemperature() = 0;
};

#endif