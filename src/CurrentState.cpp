#include "CurrentState.h"
#include <Arduino.h>
#include "Blower.h"

CurrentState* CurrentState::instance = nullptr;

CurrentState* CurrentState::get() {
    if (CurrentState::instance == nullptr) {
        CurrentState::instance = new CurrentState();
    }

    return CurrentState::instance;
}

CurrentState::CurrentState() {
    wifiConnected = false;

    isOn = false;;
    isCentralHeatingPumpOn = false;
    isHotWaterPumpOn = false;
    lighter = false;
    feederTimeToSet = 1500;
    feederPeriodToSet = 1000;
    centralHeatingTemperatureToSet = 55;
    hotWaterTemperatureToSet = 45;
    isBlowerOn = false;
    blowerSpeedToSet = Blower::Speed::RPM_0;
    centralHeatingTemperature = 0;
    hotWaterTemperature = 0;
    fumesTemperature = 0;
}
