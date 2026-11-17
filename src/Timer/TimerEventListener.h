#ifndef _TIMEREVENTLISTENER_H_
#define _TIMEREVENTLISTENER_H_

class Timer;

class TimerEventListener {
public:
    virtual void onTime(Timer* timer) = 0;
};

#endif