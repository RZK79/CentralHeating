#include "LoopTimer.h"
#include "Config.h"
#include "Controller.h"
#include "CurrentState.h"
#include "Errors.h"

LoopTimer::LoopTimer()
{
    addEventListener(this);
    start(1000);
}

void LoopTimer::startFiringUpPreblow()
{
    Controller::get()->getBlower()->setSpeed(BlowerSpeed::RPM_90);
    Controller::get()->getBlower()->start();
    Controller::get()->getMainTimer()->start(PREBLOW_TIME);
    Controller::get()->changeStateTo(Controller::State::FIRING_UP_PREBLOW);
}

void LoopTimer::startStabilization()
{
    Controller::get()->getRelays()->turnLighterOff();
    Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetStabilization);
    Controller::get()->getFeeder()->setFeedTime(0.7f * Controller::get()->getCurrentState()->feederTimeToSet);
    Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
    Controller::get()->getMainTimer()->start(STABILIZATION_TIME);
    Controller::get()->changeStateTo(Controller::State::STABILIZATION);
}

void LoopTimer::firingUpTimeout()
{
    Controller::get()->getRelays()->turnLighterOff();
    Controller::get()->getBlower()->stop();
    Controller::get()->getFeeder()->stop();
    Controller::get()->getCurrentState()->error = Errors::FIRING_UP_TIMEOUT;
    Controller::get()->getCurrentState()->isOn = false;
    Controller::get()->changeStateTo(Controller::State::OFF);
}

void LoopTimer::startExtinction()
{
    Controller::get()->getRelays()->turnCentralHeatingPumpOn();
    Controller::get()->getRelays()->turnHotWaterPumpOn();
    Controller::get()->getFeeder()->stop();
    Controller::get()->getBlower()->setSpeed(BlowerSpeed::RPM_90);
    Controller::get()->getMainTimer()->stop();
    Controller::get()->getCleaningTimer()->stop();
    Controller::get()->changeStateTo(Controller::State::EXTINCTION);
}

void LoopTimer::off()
{
    Controller::get()->getRelays()->turnCentralHeatingPumpOff();
    Controller::get()->getRelays()->turnHotWaterPumpOff();
    Controller::get()->getBlower()->stop();
    Controller::get()->getFeeder()->stop();
    Controller::get()->changeStateTo(Controller::State::OFF);
}

void LoopTimer::onTime(Timer* timer)
{
    if (Controller::get()->getState() == Controller::State::OFF) {
        if (Controller::get()->getCurrentState()->isOn && Controller::get()->getCurrentState()->fumesTemperature < FIRING_UP_MAX_TEMP && Controller::get()->getCurrentState()->centralHeatingTemperature < MINIMAL_TEMP_FOR_CH) {
            startFiringUpPreblow();
        }
    } else if (Controller::get()->getState() == Controller::State::FIRING_UP) {
        if (Controller::get()->getCurrentStateTime() < (uint32_t)Controller::get()->getCurrentState()->firingUpTimeToSet && Controller::get()->getCurrentState()->fumesTemperature > FIRING_UP_MAX_TEMP) {
            startStabilization();
        } else if (Controller::get()->getCurrentStateTime() > (uint32_t)Controller::get()->getCurrentState()->firingUpTimeToSet || ((Controller::get()->getState() != Controller::State::OFF && !Controller::get()->getCurrentState()->isOn))) {
            firingUpTimeout();
        }
    } else if (Controller::get()->getState() == Controller::State::NORMAL) {
        if (Controller::get()->getCurrentState()->centralHeatingTemperature > MINIMAL_TEMP_FOR_CH) {
            if (Controller::get()->getCurrentState()->centralHeatingTemperature > Controller::get()->getCurrentState()->centralHeatingTemperatureToSet) {
                Controller::get()->getRelays()->turnCentralHeatingPumpOff();
            } else {
                Controller::get()->getRelays()->turnCentralHeatingPumpOn();
            }
        }

        if (Controller::get()->getCurrentState()->centralHeatingTemperature > MINIMAL_TEMP_FOR_HW) {
            if (Controller::get()->getCurrentState()->hotWaterTemperature < Controller::get()->getCurrentState()->hotWaterTemperatureToSet) {
                Controller::get()->getRelays()->turnHotWaterPumpOn();
            } else {
                Controller::get()->getRelays()->turnHotWaterPumpOff();
            }
        }

        if (Controller::get()->getCurrentState()->fumesTemperature > FUMES_H_MODULATION_TEMP) {
            if (Controller::get()->getCurrentState()->centralHeatingTemperature < Controller::get()->getCurrentState()->centralHeatingTemperatureToSet || Controller::get()->getCurrentState()->hotWaterTemperature < Controller::get()->getCurrentState()->hotWaterTemperatureToSet) {
                Controller::get()->getFeeder()->setFeedTime(0.89f * Controller::get()->getCurrentState()->feederTimeToSet);
                Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
            }
            
            if(Controller::get()->getCurrentState()->centralHeatingTemperature > Controller::get()->getCurrentState()->centralHeatingTemperatureToSet){
                if(Controller::get()->getCurrentState()->hotWaterTemperature > Controller::get()->getCurrentState()->hotWaterTemperatureToSet){
                        startExtinction();
                }
            }
        }

        if (Controller::get()->getCurrentState()->fumesTemperature < FUMES_L_MODULATION_TEMP) {
            Controller::get()->getFeeder()->setFeedTime(Controller::get()->getCurrentState()->feederTimeToSet);
            Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
        }


    } else if (Controller::get()->getState() == Controller::State::EXTINCTION) {
        if (Controller::get()->getCurrentState()->fumesTemperature < FUMES_MIN_TEMP) {
            off();
        }
    }

    if(Controller::get()->getCurrentState()->centralHeatingTemperature > SAFE_CH_TEMP || Controller::get()->getCurrentState()->fumesTemperature > FUMES_MAX_TEMP){
        startExtinction();
    }

    if (Controller::get()->getState() != Controller::State::OFF && Controller::get()->getCurrentState()->isOn == false) {
        startExtinction();
    }
}