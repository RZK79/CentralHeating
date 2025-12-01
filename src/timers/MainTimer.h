#ifndef _MAINTIMER_H_
#define _MAINTIMER_H_

#include "Timer.h"
#include "TimerEventListener.h"

class MainTimer : public TimerEventListener, public Timer {
    void startFiringUp();
    void startPrefeed();
    void stabilizationTimeout();
    void startNormal();
public:
    MainTimer();
    void onTime(Timer* timer) override;
};

#endif