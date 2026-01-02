#include "CurrentState.h"
#include <Arduino.h>
#include "peripherals/Blower.h"
#include "Errors.h"
#include "Config.h"

void CurrentState::setDefault() {
    wifiConnected = false;

    isOn = false;
    isCentralHeatingPumpOn = false;
    isHotWaterPumpOn = false;
    lighter = false;
    feederTimeToSet = 1500;
    feederPeriodToSet = 5000;
    centralHeatingTemperatureToSet = 55;
    hotWaterTemperatureToSet = 45;
    blowerSpeedToSetFiringUp = BlowerSpeed::RPM_20;
    blowerSpeedToSetStabilization = BlowerSpeed::RPM_30;
    blowerSpeedToSetNormal = BlowerSpeed::RPM_20;
    centralHeatingTemperature = 0;
    hotWaterTemperature = 0;
    fumesTemperature = 0;
    firingUpTimeToSet = 4 * MINUTE;
    stabilizationTimeToSet = 3 * MINUTE;
    error = Errors::OK;
}
