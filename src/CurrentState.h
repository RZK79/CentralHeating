#ifndef _CURRENTSTATE_H_
#define _CURRENTSTATE_H_

class CurrentState {
public:
    CurrentState();

    bool wifiConnected;

    bool isOn;
    bool isCentralHeatingPumpOn;
    bool isHotWaterPumpOn;

    bool lighter;

    int feederTimeToSet;
    int feederPeriodToSet;

    int centralHeatingTemperatureToSet;
    int hotWaterTemperatureToSet;

    int blowerSpeedToSetFiringUp;
    int blowerSpeedToSetStabilization;
    int blowerSpeedToSetNormal;

    int centralHeatingTemperature;
    int hotWaterTemperature;
    int fumesTemperature;

    int NTCch;
    int NTChw;

    int firingUpTimeToSet;
    int stabilizationTimeToSet;

    int error;
};

#endif