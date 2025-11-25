#include "Config.h"
#include "Controller.h"
#include "SerialCommunication.h"
#include "peripherals/Relays.h"
#include "Errors.h"

Controller::Controller() {
    setup();
}

void Controller::setup() {
    currentState = new CurrentState();

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
}

void Controller::loop() {
    feeder->update();
    mainTimer->update();
    cleaningTimer->update();
    loopTimer->update();
    blower->update();

    if (Serial.available()) {
        SerialCommunication::get()->serialEvent();
    }
}

void Controller::onTime(Timer* timer) {
    if (timer == mainTimer) {

    } else if (timer == cleaningTimer) {

    } else if (timer == loopTimer) {


        currentStateTime++;

        getSensorsData();
    }
}

void Controller::getSensorsData() {
    currentState->fumesTemperature = fumesTemperature->getTemperature();
    currentState->centralHeatingTemperature = boilerTemperature->getTemperature();
    currentState->hotWaterTemperature = hotWaterTankTemperature->getTemperature();
}

void Controller::changeStateTo(State newState) {
    state = newState;
    currentStateTime = 0;
}

Controller::State Controller::getState() {
    return state;
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

Controller* controller = new Controller();
