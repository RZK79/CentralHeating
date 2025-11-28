#ifndef _CLEANINGTIMER_H_
#define _CLEANINGTIMER_H_

#include "Timer.h"

class CleaningTimer : public Timer, public TimerEventListener {
public:
    CleaningTimer();
    void onTime(Timer* timer) override;
};

#endif