#ifndef _FEEDER_H_
#define _FEEDER_H_

#include <Arduino.h>
#include "Timer.h"
#include "TimerEventListener.h"

class Feeder : public TimerEventListener {
public:
    enum State {
        OFF,
        PREFEED,
        FEED,
        PERIOD
    };
private:    
    Timer feederTimer;
    uint16_t feedTime;
    uint16_t periodTime;

    Feeder::State currentState;
    bool feederOn;
public:
    Feeder();

    void update();

    void prefeed();

    void setFeedTime(uint16_t feed);
    void setPeriodTime(uint16_t period);

    void start();
    void stop();
    Feeder::State getState();

    bool isFeederOn();

    virtual void onTime(Timer* timer) override;
};

#endif