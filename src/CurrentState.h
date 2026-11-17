#ifndef _CURRENTSTATE_H_
#define _CURRENTSTATE_H_

#include <stdint.h>

class CurrentState {
public:
    bool wifiConnected;

    bool isOn;
    bool isCentralHeatingPumpOn;
    bool isHotWaterPumpOn;

    bool lighter;

    uint32_t feederTimeToSet;
    uint32_t feederPeriodToSet;
    uint32_t prefeedTime;
    
    uint32_t centralHeatingTemperatureToSet; //temperature to stop CH pump
    uint32_t hotWaterTemperatureToSet;   //temperature to stop HW pump

    uint32_t blowerSpeedToSetFiringUp;
    uint32_t blowerSpeedToSetStabilization;
    uint32_t blowerSpeedToSetNormal;
    
    uint32_t centralHeatingTemperature;
    uint32_t hotWaterTemperature;
    uint32_t fumesTemperature;
    
    uint32_t NTCch;
    uint32_t NTChw;
    
    uint32_t firingUpTimeToSet;
    uint32_t stabilizationTimeToSet;
    
    uint32_t error;
    
    void setDefault();
};

#endif