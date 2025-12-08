#ifndef _BLOWER_H_
#define _BLOWER_H_

#include <Arduino.h>
#include "BlowerSpeed.h"

class Blower {
private:
    int speed;
    static int currentRPS;
    static int time;
    static int currentRPM;
public:

    Blower();
    void start();
    void stop();
    void setSpeed(int speed);
    int getSpeed();
    void update();
    bool isOn();
    static void SpeedInterrupt();
};

#endif