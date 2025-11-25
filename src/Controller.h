#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <Arduino.h>
#include "peripherals/Feeder.h"
#include "peripherals/Blower.h"
#include "peripherals/ThermoCouple.h"
#include "peripherals/NTC.h"
#include "peripherals/Relays.h"
#include "timers/MainTimer.h"
#include "timers/LoopTimer.h"
#include "timers/CleaningTimer.h"
#include "CurrentState.h"

class Controller {
public:
    enum State {
        OFF,
        FIRING_UP_PREBLOW,
        FIRING_UP_PREBLOW_DONE,
        PREFEED,
        PREFEED_DONE,
        FIRING_UP,
        STABILIZATION,
        NORMAL,
        CLEANING,
        EXTINCTION
    };
private:
    float endFiringUpTemperature;

    ThermoCouple* fumesTemperature;
    NTC* boilerTemperature;
    NTC* hotWaterTankTemperature;
    Relays* relays;
    Feeder* feeder;
    Blower* blower;

    MainTimer* mainTimer;
    LoopTimer* loopTimer;
    CleaningTimer* cleaningTimer;

    State state;

    CurrentState* currentState;
    uint32_t currentStateTime;
public:
    Controller();

    void setup();
    void loop();

    void changeStateTo(State newState);

    Controller::State getState();
    Feeder* getFeeder();
    Blower* getBlower();
    Relays* getRelays();
    MainTimer* getMainTimer();
    LoopTimer* getLoopTimer();
    CleaningTimer* getCleaningTimer();
    CurrentState* getCurrentState();
    void getSensorsData();

    virtual void onTime(Timer* timer);
};

extern Controller* controller;

#endif