#ifndef _BLOWER_H_
#define _BLOWER_H_

#include <Arduino.h>

class Blower{
    uint16_t currentRPM;
public:
    Blower();
    void start();
    void stop();
    void setRPM(uint16_t rpm);
    void update();
};

#endif