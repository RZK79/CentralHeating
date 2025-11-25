#include "MainTimer.h"
#include "Controller.h"
#include "Config.h"
#include "CurrentState.h"
#include "Errors.h"

MainTimer::MainTimer() {
    addEventListener(this);
}

void MainTimer::onTime(Timer* timer) {
    if (controller->getState() == Controller::State::FIRING_UP_PREBLOW) {
        controller->getFeeder()->prefeed();

        start(PREFEED_TIME);

        controller->changeStateTo(Controller::State::PREFEED);
    } else if (controller->getState() == Controller::State::PREFEED) {
        controller->getBlower()->setSpeed(Blower::Speed::RPM_0);

        controller->getFeeder()->setFeedTime(1.5f * controller->getCurrentState()->feederTimeToSet);
        controller->getFeeder()->setPeriodTime(controller->getCurrentState()->feederPeriodToSet);
        controller->getFeeder()->start();

        controller->getRelays()->turnLighterOn();

        controller->changeStateTo(Controller::State::FIRING_UP);
    } else if (controller->getState() == Controller::State::STABILIZATION) {
        if (getPassedMillis() > STABILIZATION_TIME && FIRING_UP_MAX_TEMP - controller->getCurrentState()->fumesTemperature >= FIRING_UP_TEMP_DIFF) {
            controller->getRelays()->turnOffAll();

            controller->getFeeder()->stop();

            controller->getBlower()->setSpeed(Blower::Speed::RPM_3600);

            stop();
            controller->getCleaningTimer()->stop();

            controller->getCurrentState()->error = Errors::STABILIZATION_TIMEOUT;

            controller->changeStateTo(Controller::State::EXTINCTION);
        } else {
            controller->getBlower()->setSpeed(Blower::Speed::RPM_1500);

            controller->getFeeder()->setFeedTime(controller->getCurrentState()->feederTimeToSet);
            controller->getFeeder()->setPeriodTime(controller->getCurrentState()->feederPeriodToSet);

            controller->getCleaningTimer()->start(TO_CLEANING_TIME);
            stop();

            controller->changeStateTo(Controller::State::NORMAL);
        }
    }

    controller->getSensorsData();
}