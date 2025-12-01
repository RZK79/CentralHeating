#include "CleaningTimer.h"
#include "Controller.h"
#include "Config.h"

CleaningTimer::CleaningTimer() {
    addEventListener(this);
}

void CleaningTimer::onTime(Timer* timer) {
    if (controller->getState() == Controller::State::CLEANING) {
        controller->getBlower()->setSpeed(Blower::Speed::RPM_1500);
        controller->getFeeder()->setFeedTime(controller->getCurrentState()->feederTimeToSet);
        controller->getFeeder()->setPeriodTime(controller->getCurrentState()->feederPeriodToSet);
        controller->getFeeder()->start();
        controller->changeStateTo(Controller::State::NORMAL);
        start(TO_CLEANING_TIME);
    } else {
        controller->getBlower()->setSpeed(Blower::Speed::RPM_3600);
        controller->getFeeder()->stop();
        controller->changeStateTo(Controller::State::CLEANING);
        start(CLEANING_TIME);
    }
}