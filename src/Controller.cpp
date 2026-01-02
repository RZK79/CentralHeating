#include "Controller.h"
#include "Config.h"
#include "Errors.h"
#include "SerialCommunication.h"
#include "peripherals/Relays.h"

Controller* Controller::instance = nullptr;

Controller* Controller::get()
{
    if (instance == nullptr) {
        instance = new Controller();
    }

    return instance;
}

Controller::Controller()
{
    se.init();
    currentState.setDefault();
}

void Controller::setup()
{
    state = State::OFF;
    fumesTemperature.init(TC_CS);
    boilerTemperature.init(NTC::ToValue(currentState.NTCch), CO_TEMP);
    hotWaterTankTemperature.init(NTC::ToValue(currentState.NTChw), CWU_TEMP);

    feeder.init();
    feeder.stop();
    blower.init(25000);
    blower.stop();
    relays.init();
    relays.turnOffAll();

    currentStateTimer.addEventListener(this);
}

void Controller::loop()
{
    feeder.update();
    mainTimer.update();
    cleaningTimer.update();
    loopTimer.update();
    blower.update();
    currentStateTimer.update();
}

SerialCommunication* Controller::getSerialCommunication(){
    return &se;
}

void Controller::getSensorsData()
{
    currentState.fumesTemperature = fumesTemperature.getTemperature();
    currentState.centralHeatingTemperature = boilerTemperature.getTemperature();
    currentState.hotWaterTemperature = hotWaterTankTemperature.getTemperature();
}

void Controller::changeStateTo(State newState)
{
    currentStateTime = 0;
    currentStateTimer.start(SECOND_MILL);
    state = newState;
}

Controller::State Controller::getState()
{
    return state;
}

const char* Controller::getStateAsString()
{
    switch (state) {
    default:
    case OFF:
        return "OFF";

    case FIRING_UP_PREBLOW:
        return "FIRING_UP_PREBLOW";

    case FIRING_UP_PREBLOW_DONE:
        return "FIRING_UP_PREBLOW_DONE";

    case PREFEED:
        return "PREFEED";

    case PREFEED_DONE:
        return "PREFEED_DONE";

    case FIRING_UP:
        return "FIRING_UP";

    case STABILIZATION:
        return "STABILIZATION";

    case NORMAL:
        return "NORMAL";

    case CLEANING:
        return "CLEANING";


    case EXTINCTION:
        return "EXTINCTION";
    }
}

Feeder* Controller::getFeeder()
{
    return &feeder;
}

Blower* Controller::getBlower()
{
    return &blower;
}

MainTimer* Controller::getMainTimer()
{
    return &mainTimer;
}

LoopTimer* Controller::getLoopTimer()
{
    return &loopTimer;
}

CleaningTimer* Controller::getCleaningTimer()
{
    return &cleaningTimer;
}

CurrentState* Controller::getCurrentState()
{
    return &currentState;
}

Relays* Controller::getRelays()
{
    return &relays;
}

uint32_t Controller::getCurrentStateTime()
{
    return currentStateTime;
}

void Controller::onTime(Timer* timer)
{
    currentStateTime++;
    getSensorsData();
    /**
     * FIRING UP SIMULATION
     */
    // if (state != State::OFF && state != State::EXTINCTION && currentState->fumesTemperature < 120) {
    //     currentState->fumesTemperature++;
    // } else if (state == State::EXTINCTION && currentState->fumesTemperature >= 50) {
    //     currentState->fumesTemperature--;
    // } else {
    //     if (state == Controller::State::NORMAL) {
    //         if (currentState->fumesTemperature == 120) {
    //             if (currentState->centralHeatingTemperature < currentState->centralHeatingTemperatureToSet) {
    //                 currentState->centralHeatingTemperature++;
    //             }

    //             if (currentState->isHotWaterPumpOn) {
    //                 if (currentState->hotWaterTemperature < currentState->hotWaterTemperatureToSet) {
    //                     currentState->hotWaterTemperature++;
    //                 }
    //             }
    //         }
    //     }
    // }

    // char buf[128];
    // sprintf(buf, "%03us ft:%02d cht:%02d hwt:%02d chp:%d hwp:%d l:%d %s state:%s",
    //     (unsigned int)currentStateTime,
    //     currentState->fumesTemperature,
    //     currentState->centralHeatingTemperature,
    //     currentState->hotWaterTemperature,
    //     currentState->isCentralHeatingPumpOn,
    //     currentState->isHotWaterPumpOn,
    //     currentState->lighter,
    //     blower->getSpeedAsString(),
    //     getStateAsString());
    // Serial.println(buf);
}
