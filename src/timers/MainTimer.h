#ifndef _MAINTIMER_H_
#define _MAINTIMER_H_

#include "Timer.h"
#include "TimerEventListener.h"

class MainTimer : public TimerEventListener, public Timer {
public:
    MainTimer();

    void onTime(Timer* timer) override;
};

#endif