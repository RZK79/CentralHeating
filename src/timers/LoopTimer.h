#ifndef _LOOPTIMER_H_
#define _LOOPTIMER_H_

#include "Timer.h"

class LoopTimer : public Timer, public TimerEventListener {
    void startFiringUpPreblow();
    void startStabilization();
    void firingUpTimeout();
    void startExtinction();
    void off();
public:
    LoopTimer();
    void onTime(Timer* timer) override;
};

#endif