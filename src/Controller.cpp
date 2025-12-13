#include "Config.h"
#include "Controller.h"
#include "SerialCommunication.h"
#include "peripherals/Relays.h"
#include "Errors.h"

Controller::Controller() {
    se = new SerialCommunication();
    currentState = new CurrentState();
}

void Controller::setup() {
    state = State::OFF;

    fumesTemperature = new ThermoCouple(TC_CS);
    boilerTemperature = new NTC(10000, CO_TEMP);
    hotWaterTankTemperature = new NTC(5000, CWU_TEMP);

    feeder = new Feeder();
    feeder->stop();
    blower = new Blower();
    blower->stop();
    relays = new Relays();
    relays->turnOffAll();

    mainTimer = new MainTimer();
    cleaningTimer = new CleaningTimer();
    loopTimer = new LoopTimer();
    currentStateTimer = new Timer();
    currentStateTimer->addEventListener(this);
}

void Controller::loop() {
    feeder->update();
    mainTimer->update();
    cleaningTimer->update();
    loopTimer->update();
    blower->update();
    currentStateTimer->update();
    se->serialEvent();
}

void Controller::getSensorsData() {
    currentState->fumesTemperature = fumesTemperature->getTemperature();
    currentState->centralHeatingTemperature = boilerTemperature->getTemperature();
    currentState->hotWaterTemperature = hotWaterTankTemperature->getTemperature();
}

void Controller::changeStateTo(State newState) {
    currentStateTime = 0;
    currentStateTimer->start(SECOND_MILL);
    state = newState;
}

Controller::State Controller::getState() {
    return state;
}

const char* Controller::getStateAsString() {
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

Feeder* Controller::getFeeder() {
    return feeder;
}

Blower* Controller::getBlower() {
    return blower;
}

MainTimer* Controller::getMainTimer() {
    return mainTimer;
}

LoopTimer* Controller::getLoopTimer() {
    return loopTimer;
}

CleaningTimer* Controller::getCleaningTimer() {
    return cleaningTimer;
}

CurrentState* Controller::getCurrentState() {
    return currentState;
}

Relays* Controller::getRelays() {
    return relays;
}

uint32_t Controller::getCurrentStateTime() {
    return currentStateTime;
}

void Controller::onTime(Timer* timer) {
    currentStateTime++;

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

Controller* controller = new Controller();
