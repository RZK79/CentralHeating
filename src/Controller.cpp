#include "Config.h"
#include "Controller.h"
#include "Timer.h"
#include "SerialCommunication.h"
#include "CurrentState.h"
#include "Relays.h"

Controller* Controller::instance = nullptr;

Controller::Controller() {
    setup();
}

Controller* Controller::get() {
    if (Controller::instance == nullptr) {
        Controller::instance = new Controller();
    }

    return Controller::instance;
}

void Controller::setup() {
    state = State::OFF;

    fumesTemperature = new ThermoCouple(TC_CS);
    boilerTemperature = new NTC(10000, CO_TEMP);
    hotWaterTankTemperature = new NTC(5000, CWU_TEMP);

    feeder = new Feeder();
    feeder->stop();

    blower = new Blower();
    blower->setSpeed(Blower::Speed::RPM_0);
    blower->stop();

    mainTimer = new Timer();
    mainTimer->addEventListener(this);

    cleaningTimer = new Timer();
    cleaningTimer->addEventListener(this);

    loopTimer = new Timer();
    loopTimer->addEventListener(this);
    loopTimer->start(1000);

    Relays::get()->turnOffAll();

    feederMultiplier = 1.0f;
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
        if (state == State::FIRING_UP_PREBLOW) {
            feeder->prefeed();

            mainTimer->start(PREFEED_TIME);

            changeStateTo(State::PREFEED);
        } else if (state == State::PREFEED) {
            blower->setSpeed(Blower::Speed::RPM_0);

            feeder->setFeedTime(1.5f * CurrentState::get()->feederTimeToSet);
            feeder->setPeriodTime(CurrentState::get()->feederPeriodToSet);
            feeder->start();

            Relays::get()->turnLighterOn();

            changeStateTo(State::FIRING_UP);
        } else if (state == State::STABILIZATION) {
            if (FIRING_UP_MAX_TEMP - CurrentState::get()->fumesTemperature >= FIRING_UP_TEMP_DIFF) {
                Relays::get()->turnOffAll();

                feeder->stop();

                blower->setSpeed(Blower::Speed::RPM_3600);

                mainTimer->stop();
                cleaningTimer->stop();

                changeStateTo(State::EXTINCTION);
            } else {
                blower->setSpeed(Blower::Speed::RPM_1500);

                feeder->setFeedTime(CurrentState::get()->feederTimeToSet);
                feeder->setPeriodTime(CurrentState::get()->feederPeriodToSet);

                cleaningTimer->start(TO_CLEANING_TIME);
                mainTimer->stop();

                changeStateTo(State::NORMAL);
            }
        }
    } else if (timer == cleaningTimer) {
        if (state == State::CLEANING) {
            blower->setSpeed(Blower::Speed::RPM_1500);

            feeder->setFeedTime(CurrentState::get()->feederTimeToSet);
            feeder->setPeriodTime(CurrentState::get()->feederPeriodToSet);
            feeder->start();

            cleaningTimer->start(TO_CLEANING_TIME);

            changeStateTo(State::NORMAL);
        } else {
            blower->setSpeed(Blower::Speed::RPM_3600);

            feeder->stop();

            cleaningTimer->start(CLEANING_TIME);

            changeStateTo(State::CLEANING);
        }
    } else if (timer == loopTimer) {
        if (state == State::OFF) {
            if (CurrentState::get()->isOn) {
                blower->setSpeed(Blower::Speed::RPM_3600);
                blower->start();

                mainTimer->start(PREBLOW_TIME);

                changeStateTo(State::FIRING_UP_PREBLOW);
            }
        } else if (state == State::FIRING_UP) {
            if (CurrentState::get()->fumesTemperature > FIRING_UP_MAX_TEMP) {
                Relays::get()->turnLighterOff();

                blower->setSpeed(Blower::Speed::RPM_2000);

                feeder->setFeedTime(CurrentState::get()->feederTimeToSet);
                feeder->setPeriodTime(CurrentState::get()->feederPeriodToSet);

                mainTimer->start(STABILIZATION_TIME);

                changeStateTo(State::STABILIZATION);
            }
        } else if (state == State::NORMAL) {
            if (CurrentState::get()->centralHeatingTemperature > 40) {
                if (CurrentState::get()->centralHeatingTemperature < CurrentState::get()->centralHeatingTemperatureToSet) {
                    Relays::get()->turnCentralHeatingPumpOn();
                } else {
                    Relays::get()->turnCentralHeatingPumpOff();
                }
            }

            if (CurrentState::get()->centralHeatingTemperature > 50) {
                if (CurrentState::get()->centralHeatingTemperature > CurrentState::get()->centralHeatingTemperatureToSet) {
                    Relays::get()->turnCentralHeatingPumpOn();
                } else {
                    Relays::get()->turnHotWaterPumpOff();
                }
            }

            if (CurrentState::get()->fumesTemperature > 200) {
                feederMultiplier *= 0.8f;
                feeder->setFeedTime(feederMultiplier * CurrentState::get()->feederTimeToSet);
                feeder->setPeriodTime(CurrentState::get()->feederPeriodToSet);
            }
        } else if (state == State::EXTINCTION) {
            if (CurrentState::get()->fumesTemperature < 70) {
                blower->stop();

                CurrentState::get()->isOn = false;

                changeStateTo(State::OFF);
            }
        } else if (state != State::OFF && CurrentState::get()->isOn == false) {
            Relays::get()->turnOffAll();

            feeder->stop();

            blower->setSpeed(Blower::Speed::RPM_3600);

            mainTimer->stop();
            cleaningTimer->stop();

            changeStateTo(State::EXTINCTION);
        }

        getSensorsData();
    }
}

void Controller::getSensorsData() {
    CurrentState::get()->fumesTemperature = fumesTemperature->getTemperature();
    CurrentState::get()->centralHeatingTemperature = boilerTemperature->getTemperature();
    CurrentState::get()->hotWaterTemperature = hotWaterTankTemperature->getTemperature();
}

void Controller::changeStateTo(State newState) {
    state = newState;
}

State Controller::getState() {
    return state;
}
