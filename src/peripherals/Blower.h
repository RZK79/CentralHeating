#ifndef _BLOWER_H_
#define _BLOWER_H_

#include <Arduino.h>
#include "Updateable.h"

// #define ENABLE_ENCODER

class Blower : public Updateable {
private:
#ifdef ENABLE_ENCODER
    static int currentRPS;
    static int time;
    static int currentRPM;
#endif
    int speed;
public:
    Blower(int PWMfreq);
    void start();
    void stop();
    void setSpeed(int speed);
    int getSpeed();
    virtual void update() override;
    bool isOn();
#ifdef ENABLE_ENCODER
    static void SpeedInterrupt();
#endif
};

#endif