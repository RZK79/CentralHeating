#ifndef _NTC_H_
#define _NTC_H_

#include <Arduino.h>
#include "TemperatureSensor.h"
#include "NTCType.h"

class NTC : public TemperatureSensor {
    uint32_t RT0;       //thermistor resistance in 25 celsius degrees
    uint32_t B = 3470;  //thermistor material constant
    float Ucc = 5.0f;   //for Arduino it's 5V
    uint32_t R = 10000; //second resistor from voltage divider
    float readSensor();
public:
    void init(int resistance, const uint8_t analogPin, uint16_t Bvalue = 3470);
    float getTemperature() override;
    static int ToValue(int type);
};

#endif