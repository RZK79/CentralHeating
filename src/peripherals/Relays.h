#ifndef _RELAYS_H_
#define _RELAYS_H_

#include <Arduino.h>

class Relays {
public:
    void init();
    void turnOffAll();

    void turnLighterOn();
    void turnLighterOff();
    bool isLighterOn();

    void turnCentralHeatingPumpOn();
    void turnCentralHeatingPumpOff();
    bool isCentralHeatingPumpOn();

    void turnHotWaterPumpOn();
    void turnHotWaterPumpOff();
    bool isHotWaterPumpOn();
};

#endif