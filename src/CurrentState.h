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

    int blowerSpeedToSet;

    int centralHeatingTemperature;
    int hotWaterTemperature;
    int fumesTemperature;

    int error;
};

#endif