#ifndef _CONTROLLERSTATE_H_
#define _CONTROLLERSTATE_H_

#include <Arduino.h>
#include "TimerEventListener.h"
#include "Feeder.h"
#include "Blower.h"
#include "ThermoCouple.h"
#include "NTC.h"

enum State {
    OFF,
    FIRING_UP_PREBLOW,
    PREFEED,
    FIRING_UP,
    STABILIZATION,
    NORMAL,
    CLEANING,
    EXTINCTION
};

class Controller : public TimerEventListener {
    static Controller* instance;
    Controller();

    float endFiringUpTemperature;

    ThermoCouple* fumesTemperature;
    NTC* boilerTemperature;
    NTC* hotWaterTankTemperature;

    Feeder* feeder;
    Blower* blower;

    Timer* mainTimer;
    Timer* loopTimer;
    Timer* cleaningTimer;

    float feederMultiplier;

    State state;

    uint32_t currentStateTime;
public:
    static Controller* get();

    void setup();
    void loop();

    void changeStateTo(State newState);

    State getState();
    bool isBlowerOn();
    bool isFeederOn();

    void getSensorsData();

    virtual void onTime(Timer* timer);
};

#endif