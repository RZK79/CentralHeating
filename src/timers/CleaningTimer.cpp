#include "CleaningTimer.h"
#include "Config.h"
#include "Controller.h"

#define CLEANING_TIME 10*1000

CleaningTimer::CleaningTimer()
{
    addEventListener(this);
}

void CleaningTimer::onTime(Timer* timer)
{
    if (Controller::get()->getState() == Controller::State::CLEANING) {
        Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetNormal);
        Controller::get()->getFeeder()->setFeedTime(Controller::get()->getCurrentState()->feederTimeToSet);
        Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
        Controller::get()->getFeeder()->start();
        Controller::get()->changeStateTo(Controller::State::NORMAL);
        start(TO_CLEANING_TIME);
    } else {
        Controller::get()->getBlower()->setSpeed(90);
        Controller::get()->getFeeder()->stop();
        Controller::get()->changeStateTo(Controller::State::CLEANING);
        start(CLEANING_TIME);
    }
}