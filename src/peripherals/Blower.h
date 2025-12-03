#ifndef _BLOWER_H_
#define _BLOWER_H_

#include <Arduino.h>
#include "BlowerSpeed.h"

class Blower {
private:
    BlowerSpeed speed;
    static int currentRPS;
    static int time;
    static int currentRPM;
public:

    Blower();
    void start();
    void stop();
    void setSpeed(BlowerSpeed speed);
    BlowerSpeed getSpeed();
    void update();
    bool isOn();
    static void SpeedInterrupt();
};

#endif