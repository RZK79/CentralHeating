#include "LoopTimer.h"
#include "Config.h"
#include "Controller.h"
#include "CurrentState.h"
#include "Errors.h"

#define PREBLOW_TIME 10
#define FIRING_UP_MAX_TEMP 90
#define FUMES_MIN_TEMP 80
#define FUMES_L_MODULATION_TEMP 120
#define FUMES_ML_MODULATION_TEMP 140
#define FUMES_MH_MODULATION_TEMP 160
#define FUMES_H_MODULATION_TEMP 170
#define FUMES_MAX_TEMP 250
#define FIRING_UP_TEMP_DIFF 10
#define SAFE_CH_TEMP 70
#define EXTINCTION_PHASE_TEMP 140

#define FIRING_UP_TIME              10*60
#define STABILIZATION_TIME          3*60

LoopTimer::LoopTimer()
{
    addEventListener(this);
    start(1000);
}

void LoopTimer::startFiringUpPreblow()
{
    Controller::get()->getBlower()->setSpeed(90);
    Controller::get()->getBlower()->start();
    Controller::get()->changeStateTo(Controller::State::FIRING_UP_PREBLOW);
}

void LoopTimer::startFiringUp()
{
    Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetFiringUp);
    Controller::get()->getFeeder()->setFeedTime(0.7f * Controller::get()->getCurrentState()->feederTimeToSet);
    Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
    Controller::get()->getFeeder()->start();
    Controller::get()->getRelays()->turnLighterOn();
    Controller::get()->changeStateTo(Controller::State::FIRING_UP);
}

void LoopTimer::startPrefeed()
{
    Controller::get()->getBlower()->setSpeed(0);
    Controller::get()->getFeeder()->prefeed();
    Controller::get()->changeStateTo(Controller::State::PREFEED);
    start(Controller::get()->getCurrentState()->prefeedTime);
}

void LoopTimer::stabilizationTimeout()
{
    Controller::get()->getRelays()->turnOffAll();
    Controller::get()->getFeeder()->stop();
    Controller::get()->getBlower()->setSpeed(90);
    Controller::get()->getCleaningTimer()->stop();
    Controller::get()->getCurrentState()->error = Errors::STABILIZATION_TIMEOUT;
    Controller::get()->changeStateTo(Controller::State::EXTINCTION);
    stop();
}

void LoopTimer::startNormal()
{
    Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetNormal);
    Controller::get()->getFeeder()->setFeedTime(Controller::get()->getCurrentState()->feederTimeToSet);
    Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
    Controller::get()->getCleaningTimer()->start(TO_CLEANING_TIME);
    Controller::get()->changeStateTo(Controller::State::NORMAL);
    stop();
}

void LoopTimer::startStabilization()
{
    Controller::get()->getRelays()->turnLighterOff();
    Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetStabilization);
    Controller::get()->getFeeder()->setFeedTime(0.7f * Controller::get()->getCurrentState()->feederTimeToSet);
    Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
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
    Controller::get()->getCleaningTimer()->stop();
    Controller::get()->changeStateTo(Controller::State::EXTINCTION);
}

void LoopTimer::off()
{
    Controller::get()->getBlower()->stop();
    Controller::get()->getFeeder()->stop();
    Controller::get()->getRelays()->turnCentralHeatingPumpOff();
    Controller::get()->getRelays()->turnHotWaterPumpOff();
    Controller::get()->changeStateTo(Controller::State::OFF);
}

void LoopTimer::manageCentralHeatingPump()
{
    if (Controller::get()->getCurrentState()->fumesTemperature > FUMES_L_MODULATION_TEMP) {
        if (Controller::get()->getCurrentState()->centralHeatingTemperature <= Controller::get()->getCurrentState()->centralHeatingTemperatureToSet) {
            Controller::get()->getRelays()->turnCentralHeatingPumpOn();
        } else {
            Controller::get()->getRelays()->turnCentralHeatingPumpOff();
        }
    }
}

void LoopTimer::manageHotWaterPump()
{
    if (Controller::get()->getCurrentState()->fumesTemperature > FUMES_L_MODULATION_TEMP + 10) {
        if (Controller::get()->getCurrentState()->hotWaterTemperature <= Controller::get()->getCurrentState()->hotWaterTemperatureToSet) {
            Controller::get()->getRelays()->turnHotWaterPumpOn();
        } else {
            Controller::get()->getRelays()->turnHotWaterPumpOff();
        }
    }
}

void LoopTimer::onTime(Timer* timer)
{
    if (Controller::get()->getState() == Controller::State::OFF) {
        if (Controller::get()->getCurrentState()->isOn) {
            if (Controller::get()->getCurrentState()->fumesTemperature < FUMES_MIN_TEMP) {
                if (
                    Controller::get()->getCurrentState()->centralHeatingTemperature < Controller::get()->getCurrentState()->centralHeatingTemperatureToSet || Controller::get()->getCurrentState()->hotWaterTemperature < Controller::get()->getCurrentState()->hotWaterTemperatureToSet) {
                    startFiringUpPreblow();
                }
            }
        }
    } else if (Controller::get()->getState() == Controller::State::FIRING_UP_PREBLOW) {
        if (Controller::get()->getCurrentStateTime() > PREBLOW_TIME) {
            startPrefeed();
        }
    } else if (Controller::get()->getState() == Controller::State::PREFEED) {
        if (Controller::get()->getCurrentStateTime() > Controller::get()->getCurrentState()->prefeedTime) {
            startFiringUp();
        }
    } else if (Controller::get()->getState() == Controller::State::FIRING_UP) {
        if (Controller::get()->getCurrentStateTime() <= (uint32_t)Controller::get()->getCurrentState()->firingUpTimeToSet) {
            if (Controller::get()->getCurrentState()->fumesTemperature > FIRING_UP_MAX_TEMP) {
                startStabilization();
            }
        } else {
            firingUpTimeout();
        }
    } else if (Controller::get()->getState() == Controller::State::STABILIZATION) {
        if (Controller::get()->getCurrentStateTime() > STABILIZATION_TIME) {
            if (Controller::get()->getCurrentState()->fumesTemperature < FUMES_L_MODULATION_TEMP) {
                stabilizationTimeout();
            } else {
                startNormal();
            }
        }
    } else if (Controller::get()->getState() == Controller::State::NORMAL) {
        manageCentralHeatingPump();
        manageHotWaterPump();

        if (Controller::get()->getCurrentState()->fumesTemperature > FUMES_H_MODULATION_TEMP) {
            Controller::get()->getBlower()->setSpeed((int)(0.9f * Controller::get()->getCurrentState()->blowerSpeedToSetNormal));
        }

        if (Controller::get()->getCurrentState()->fumesTemperature >= FUMES_ML_MODULATION_TEMP && Controller::get()->getCurrentState()->fumesTemperature <= FUMES_MH_MODULATION_TEMP) {
            Controller::get()->getBlower()->setSpeed(Controller::get()->getCurrentState()->blowerSpeedToSetNormal);
        }

        if (Controller::get()->getCurrentState()->fumesTemperature < FUMES_L_MODULATION_TEMP) {
            Controller::get()->getBlower()->setSpeed((int)(1.2f * Controller::get()->getCurrentState()->blowerSpeedToSetNormal));
        }

        if (Controller::get()->getCurrentState()->centralHeatingTemperature > Controller::get()->getCurrentState()->centralHeatingTemperatureToSet) {
            if (Controller::get()->getCurrentState()->hotWaterTemperature > Controller::get()->getCurrentState()->hotWaterTemperatureToSet) {
                startExtinction();
            }
        }

        if (Controller::get()->getCurrentState()->fumesTemperature < FIRING_UP_MAX_TEMP) {
            startExtinction();
        }
    } else if (Controller::get()->getState() == Controller::State::EXTINCTION) {
        if (Controller::get()->getCurrentState()->fumesTemperature > EXTINCTION_PHASE_TEMP) {
            Controller::get()->getBlower()->setSpeed(45);
        } else {
            Controller::get()->getBlower()->setSpeed(90);
        }

        if (Controller::get()->getCurrentState()->fumesTemperature < FUMES_MIN_TEMP) {
            off();
        }
    }

    if (Controller::get()->getCurrentState()->centralHeatingTemperature > SAFE_CH_TEMP) {
        Controller::get()->getCurrentState()->error = Errors::CENTRAL_HEATING_TEMPERATURE_TO_HIGH;
        startExtinction();
    }

    if (Controller::get()->getCurrentState()->fumesTemperature > FUMES_MAX_TEMP) {
        Controller::get()->getCurrentState()->error = Errors::FUMES_TEMPERATURE_TOO_HIGH;
        startExtinction();
    }

    if (Controller::get()->getState() != Controller::State::OFF && Controller::get()->getCurrentState()->isOn == false) {
        startExtinction();
    }
}