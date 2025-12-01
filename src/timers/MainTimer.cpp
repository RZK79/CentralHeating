#include "MainTimer.h"
#include "Controller.h"
#include "Config.h"
#include "CurrentState.h"
#include "Errors.h"

MainTimer::MainTimer() {
    addEventListener(this);
}

void MainTimer::startFiringUp() {
    controller->getBlower()->setSpeed(Blower::Speed::RPM_2500);
    controller->getFeeder()->setFeedTime(1.5f * controller->getCurrentState()->feederTimeToSet);
    controller->getFeeder()->setPeriodTime(controller->getCurrentState()->feederPeriodToSet);
    controller->getFeeder()->start();
    controller->getRelays()->turnLighterOn();
    controller->changeStateTo(Controller::State::FIRING_UP);
}

void MainTimer::startPrefeed() {
    controller->getBlower()->setSpeed(Blower::Speed::RPM_1000);
    controller->getFeeder()->prefeed();
    controller->changeStateTo(Controller::State::PREFEED);
    start(PREFEED_TIME);
}

void MainTimer::stabilizationTimeout() {
    controller->getRelays()->turnOffAll();
    controller->getFeeder()->stop();
    controller->getBlower()->setSpeed(Blower::Speed::RPM_3600);
    controller->getCleaningTimer()->stop();
    controller->getCurrentState()->error = Errors::STABILIZATION_TIMEOUT;
    controller->changeStateTo(Controller::State::EXTINCTION);
    stop();
}

void MainTimer::startNormal() {
    controller->getBlower()->setSpeed(Blower::Speed::RPM_2000);
    controller->getFeeder()->setFeedTime(controller->getCurrentState()->feederTimeToSet);
    controller->getFeeder()->setPeriodTime(controller->getCurrentState()->feederPeriodToSet);
    controller->getCleaningTimer()->start(TO_CLEANING_TIME);
    controller->changeStateTo(Controller::State::NORMAL);
    stop();
}

void MainTimer::onTime(Timer* timer) {
    if (controller->getState() == Controller::State::FIRING_UP_PREBLOW) {
        startPrefeed();
    } else if (controller->getState() == Controller::State::PREFEED) {
        startFiringUp();
    } else if (controller->getState() == Controller::State::STABILIZATION) {
        if (controller->getCurrentStateTime() > STABILIZATION_TIME) {
            if (controller->getCurrentState()->fumesTemperature - FIRING_UP_MAX_TEMP < FIRING_UP_TEMP_DIFF) {
                stabilizationTimeout();
            } else {
                startNormal();
            }
        }
        controller->getSensorsData();
    }
}