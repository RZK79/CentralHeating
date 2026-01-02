#ifndef _BLOWER_H_
#define _BLOWER_H_

#include <Arduino.h>
#include "BlowerSpeed.h"

// #define ENABLE_ENCODER

class Blower {
private:
    static int currentRPS;
    static int time;
    static int currentRPM;
    int freq;
public:
    static float fullPeriod; 
    static int speed;
    void init(int PWMfreq);
    void start();
    void stop();
    void setSpeed(int speed);
    int getSpeed();
    void update();
    bool isOn();
#ifdef ENABLE_ENCODER
    static void SpeedInterrupt();
#endif
};

#endif