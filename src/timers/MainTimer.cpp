#include "MainTimer.h"
#include "Config.h"
#include "Controller.h"
#include "CurrentState.h"
#include "Errors.h"

MainTimer::MainTimer()
{
    addEventListener(this);
}

void MainTimer::startFiringUp()
{
    Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetFiringUp);
    Controller::get()->getFeeder()->setFeedTime(0.7f * Controller::get()->getCurrentState()->feederTimeToSet);
    Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
    Controller::get()->getFeeder()->start();
    Controller::get()->getRelays()->turnLighterOn();
    Controller::get()->changeStateTo(Controller::State::FIRING_UP);
}

void MainTimer::startPrefeed()
{
    Controller::get()->getBlower()->setSpeed(0);
    Controller::get()->getFeeder()->prefeed();
    Controller::get()->changeStateTo(Controller::State::PREFEED);
    start(PREFEED_TIME);
}

void MainTimer::stabilizationTimeout()
{
    Controller::get()->getRelays()->turnOffAll();
    Controller::get()->getFeeder()->stop();
    Controller::get()->getBlower()->setSpeed(90);
    Controller::get()->getCleaningTimer()->stop();
    Controller::get()->getCurrentState()->error = Errors::STABILIZATION_TIMEOUT;
    Controller::get()->changeStateTo(Controller::State::EXTINCTION);
    stop();
}

void MainTimer::startNormal()
{
    Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetNormal);
    Controller::get()->getFeeder()->setFeedTime(Controller::get()->getCurrentState()->feederTimeToSet);
    Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
    Controller::get()->getCleaningTimer()->start(TO_CLEANING_TIME);
    Controller::get()->changeStateTo(Controller::State::NORMAL);
    stop();
}

void MainTimer::onTime(Timer* timer)
{
    if (Controller::get()->getState() == Controller::State::FIRING_UP_PREBLOW) {
        startPrefeed();
    } else if (Controller::get()->getState() == Controller::State::PREFEED) {
        startFiringUp();
    } else if (Controller::get()->getState() == Controller::State::STABILIZATION) {
        if (Controller::get()->getCurrentStateTime() > (uint32_t)Controller::get()->getCurrentState()->stabilizationTimeToSet) {
            if (Controller::get()->getCurrentState()->fumesTemperature - FIRING_UP_MAX_TEMP < FIRING_UP_TEMP_DIFF) {
                stabilizationTimeout();
            } else {
                startNormal();
            }
        }
    }
}