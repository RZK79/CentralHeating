#include "CurrentState.h"
#include <Arduino.h>
#include "peripherals/Blower.h"
#include "Errors.h"

CurrentState::CurrentState() {
    wifiConnected = false;

    isOn = false;;
    isCentralHeatingPumpOn = false;
    isHotWaterPumpOn = false;
    lighter = false;
    feederTimeToSet = 2000;
    feederPeriodToSet = 3000;
    centralHeatingTemperatureToSet = 55;
    hotWaterTemperatureToSet = 45;
    blowerSpeedToSet = BlowerSpeed::RPM_0;
    centralHeatingTemperature = 0;
    hotWaterTemperature = 0;
    fumesTemperature = 0;
    error = Errors::OK;
}
