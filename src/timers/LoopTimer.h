#ifndef _LOOPTIMER_H_
#define _LOOPTIMER_H_

#include "Timer.h"

class LoopTimer : public Timer, public TimerEventListener {
    float feederMultiplier;
public:
    LoopTimer();
    virtual void onTime(Timer* timer);
};

#endif