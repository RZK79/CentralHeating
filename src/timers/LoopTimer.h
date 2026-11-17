#ifndef _LOOPTIMER_H_
#define _LOOPTIMER_H_

#include "Timer/Timer.h"

class LoopTimer : public Timer, public TimerEventListener {
    void startFiringUpPreblow();
    void startPrefeed();
    void startFiringUp();
    void startStabilization();
    void stabilizationTimeout();
    void startNormal();
    void firingUpTimeout();
    void startExtinction();
    void off();
    void manageCentralHeatingPump();
    void manageHotWaterPump();
public:
    LoopTimer();
    void onTime(Timer* timer) override;
};

#endif