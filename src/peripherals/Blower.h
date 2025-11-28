#ifndef _BLOWER_H_
#define _BLOWER_H_

#include <Arduino.h>

class Blower {
public:
    enum Speed {
        RPM_3600,
        RPM_3000,
        RPM_2500,
        RPM_2000,
        RPM_1500,
        RPM_1000,
        RPM_0,
        RPM_MAX = RPM_3600
    };
private:
    Speed speed;
    static int currentRPS;
    static int time;
    static int currentRPM;
public:

    Blower();
    void start();
    void stop();
    void setSpeed(Speed speed);
    Speed getSpeed();
    const char* getSpeedAsString();
    void update();
    bool isOn();
    static void SpeedInterrupt();
};

#endif