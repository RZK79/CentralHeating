#include "LoopTimer.h"
#include "Controller.h"
#include "CurrentState.h"
#include "Config.h"
#include "Errors.h"

LoopTimer::LoopTimer() {
    addEventListener(this);
    start(1000);

    feederMultiplier = 1.0f;
}

void LoopTimer::onTime(Timer* timer) {
    if (controller->getState() == Controller::State::OFF) {
        if (controller->getCurrentState()->isOn) {
            controller->getBlower()->setSpeed(Blower::Speed::RPM_3600);
            controller->getBlower()->start();
            controller->getMainTimer()->start(PREBLOW_TIME);
            controller->changeStateTo(Controller::State::FIRING_UP_PREBLOW);
        }
    } else if (controller->getState() == Controller::State::FIRING_UP) {
        if (controller->getCurrentStateTime() < FIRING_UP_TIME && controller->getCurrentState()->fumesTemperature > FIRING_UP_MAX_TEMP) {
            controller->getRelays()->turnLighterOff();
            controller->getBlower()->setSpeed(Blower::Speed::RPM_2000);
            controller->getFeeder()->setFeedTime(controller->getCurrentState()->feederTimeToSet);
            controller->getFeeder()->setPeriodTime(controller->getCurrentState()->feederPeriodToSet);
            controller->getMainTimer()->start(STABILIZATION_TIME);
            controller->changeStateTo(Controller::State::STABILIZATION);
        } else if (controller->getCurrentStateTime() > FIRING_UP_TIME || ((controller->getState() != Controller::State::OFF && !controller->getCurrentState()->isOn))) {
            controller->getRelays()->turnLighterOff();
            controller->getBlower()->stop();
            controller->getFeeder()->stop();
            controller->getCurrentState()->error = Errors::FIRING_UP_TIMEOUT;
            controller->getCurrentState()->isOn = false;
            controller->changeStateTo(Controller::State::OFF);
            Serial.println("Errors::FIRING_UP_TIMEOUT");
        }
    } else if (controller->getState() == Controller::State::NORMAL) {
        if (controller->getCurrentState()->centralHeatingTemperature > 40) {
            if (controller->getCurrentState()->centralHeatingTemperature > controller->getCurrentState()->centralHeatingTemperatureToSet) {
                controller->getRelays()->turnCentralHeatingPumpOff();
            }else{
                controller->getRelays()->turnCentralHeatingPumpOn();
            }
        }

        if (controller->getCurrentState()->centralHeatingTemperature > 50) {
            if (controller->getCurrentState()->hotWaterTemperature < controller->getCurrentState()->hotWaterTemperatureToSet) {
                controller->getRelays()->turnHotWaterPumpOn();
            } else {
                controller->getRelays()->turnHotWaterPumpOff();
            }
        }

        if (controller->getCurrentState()->fumesTemperature > 200) {
            if (controller->getCurrentState()->centralHeatingTemperature < controller->getCurrentState()->centralHeatingTemperatureToSet ||
                controller->getCurrentState()->hotWaterTemperature < controller->getCurrentState()->hotWaterTemperatureToSet) {
                feederMultiplier *= 0.8f;
                controller->getFeeder()->setFeedTime(feederMultiplier * controller->getCurrentState()->feederTimeToSet);
                controller->getFeeder()->setPeriodTime(controller->getCurrentState()->feederPeriodToSet);
            } else {

            }
        }
    } else if (controller->getState() == Controller::State::EXTINCTION) {
        if (controller->getCurrentState()->fumesTemperature < 50) {
            controller->getBlower()->stop();
            controller->changeStateTo(Controller::State::OFF);
        }
    } else if (controller->getState() != Controller::State::OFF && !controller->getCurrentState()->isOn) {
        controller->getRelays()->turnOffAll();
        controller->getFeeder()->stop();
        controller->getBlower()->setSpeed(Blower::Speed::RPM_3600);
        controller->getMainTimer()->stop();
        controller->getCleaningTimer()->stop();
        controller->changeStateTo(Controller::State::EXTINCTION);
    }
}