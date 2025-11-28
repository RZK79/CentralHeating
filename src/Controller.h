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
#include "SerialCommunication.h"

class Controller : public TimerEventListener{
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
    ThermoCouple* fumesTemperature;
    NTC* boilerTemperature;
    NTC* hotWaterTankTemperature;
    Relays* relays;
    Feeder* feeder;
    Blower* blower;

    Timer* currentStateTimer;
    MainTimer* mainTimer;
    LoopTimer* loopTimer;
    CleaningTimer* cleaningTimer;

    State state;

    CurrentState* currentState;

    SerialCommunication* se;
    uint32_t currentStateTime;    
public:
    Controller();

    void setup();
    void loop();

    void changeStateTo(State newState);

    Controller::State getState();
    const char* getStateAsString();
    Feeder* getFeeder();
    Blower* getBlower();
    Relays* getRelays();
    MainTimer* getMainTimer();
    LoopTimer* getLoopTimer();
    CleaningTimer* getCleaningTimer();
    CurrentState* getCurrentState();
    uint32_t getCurrentStateTime();
    void getSensorsData();
    void onTime(Timer* timer) override;
};

extern Controller* controller;

#endif