#include "CurrentState.h"
#include "Errors.h"

void CurrentState::setDefault()
{
    wifiConnected = false;

    isOn = false;
    isCentralHeatingPumpOn = false;
    isHotWaterPumpOn = false;
    lighter = false;
    feederTimeToSet = 1100;
    feederPeriodToSet = 9000;
    centralHeatingTemperatureToSet = 60;
    hotWaterTemperatureToSet = 50;
    blowerSpeedToSetFiringUp = 90;
    blowerSpeedToSetStabilization = 50;
    blowerSpeedToSetNormal = 40;
    centralHeatingTemperature = 0;
    hotWaterTemperature = 0;
    fumesTemperature = 0;
    firingUpTimeToSet = 4 * 60;
    prefeedTime = 20;
    stabilizationTimeToSet = 3 * 60;
    error = Errors::OK;
}
