#include "CleaningTimer.h"
#include "Controller.h"
#include "Config.h"

CleaningTimer::CleaningTimer() {
    addEventListener(this);
}

void CleaningTimer::onTime(Timer* timer) {
    if (Controller::get()->getState() == Controller::State::CLEANING) {
        Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetNormal);
        Controller::get()->getFeeder()->setFeedTime(Controller::get()->getCurrentState()->feederTimeToSet);
        Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
        Controller::get()->getFeeder()->start();
        Controller::get()->changeStateTo(Controller::State::NORMAL);
        start(TO_CLEANING_TIME);
    } else {
        Controller::get()->getBlower()->setSpeed(BlowerSpeed::RPM_100);
        Controller::get()->getFeeder()->stop();
        Controller::get()->changeStateTo(Controller::State::CLEANING);
        start(CLEANING_TIME);
    }
}