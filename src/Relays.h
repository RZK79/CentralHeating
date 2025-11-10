#ifndef _RELAYS_H_
#define _RELAYS_H_

#include <Arduino.h>

class Relays {
    static Relays* instance;
    Relays();
public:
    static Relays* get();

    void turnOffAll();

    void turnLighterOn();
    void turnLighterOff();

    void turnCentralHeatingPumpOn();
    void turnCentralHeatingPumpOff();

    void turnHotWaterPumpOn();
    void turnHotWaterPumpOff();
};

#endif