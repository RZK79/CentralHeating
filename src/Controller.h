#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <Arduino.h>
#include <vector>
#include "CurrentState.h"
#include "SerialCommunication.h"
#include "peripherals/Blower.h"
#include "peripherals/Feeder.h"
#include "peripherals/NTC.h"
#include "peripherals/Relays.h"
#include "peripherals/ThermoCouple.h"
#include "timers/CleaningTimer.h"
#include "timers/LoopTimer.h"

// #define SIMULATION

using namespace std;

class Controller : public TimerEventListener {
public:
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

private:
    static Controller* instance;

    ThermoCouple* fumesTemperature;
    NTC* boilerTemperature;
    NTC* hotWaterTankTemperature;
    Relays* relays;
    Feeder* feeder;
    Blower* blower;

    Timer* currentStateTimer;
    LoopTimer* loopTimer;
    CleaningTimer* cleaningTimer;

    CurrentState* currentState;

    SerialCommunication* se;
    uint32_t currentStateTime;

    State state;

    vector<Updateable*> updateables;

    Controller();
public:
    static Controller* get();

    void setup();
    void loop();

    void changeStateTo(State newState);

    Controller::State getState();
    Feeder* getFeeder();
    Blower* getBlower();
    Relays* getRelays();
    LoopTimer* getLoopTimer();
    CleaningTimer* getCleaningTimer();
    CurrentState* getCurrentState();
    uint32_t getCurrentStateTime();
    void getSensorsData();
    SerialCommunication* getSerialCommunication();
    void onTime(Timer* timer) override;
};

#endif