#ifndef _MAINTIMER_H_
#define _MAINTIMER_H_

#include "Timer.h"
#include "TimerEventListener.h"

class MainTimer : public TimerEventListener, public Timer {
public:
    MainTimer();

    virtual void onTime(Timer* timer);
};

#endif