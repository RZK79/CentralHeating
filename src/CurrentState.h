#ifndef _CURRENTSTATE_H_
#define _CURRENTSTATE_H_

class CurrentState {
    static CurrentState* instance;
    CurrentState();
public:
    static CurrentState* get();

    bool wifiConnected;
    
    bool isOn;
    bool isCentralHeatingPumpOn;
    bool isHotWaterPumpOn;
    
    bool lighter;

    int feederTimeToSet;
    int feederPeriodToSet;
    
    int centralHeatingTemperatureToSet;
    int hotWaterTemperatureToSet;
    
    bool isBlowerOn;
    int blowerSpeedToSet;

    int centralHeatingTemperature;
    int hotWaterTemperature;
    int fumesTemperature;
};

#endif