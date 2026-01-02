#ifndef _TEMPERATURESENSOR_H_
#define _TEMPERATURESENSOR_H_

#include <Arduino.h>

class TemperatureSensor {
protected:
    uint8_t readPin;
    float buf[10];
public:
    virtual float getTemperature() = 0;
};

#endif