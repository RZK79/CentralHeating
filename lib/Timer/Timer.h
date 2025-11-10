#ifndef _TIMER_H_
#define _TIMER_H_

#include <vector>
#include "TimerEventListener.h"

using namespace std;

class Timer{
  unsigned long timeout;
  unsigned long startMillis;
  bool paused;

  vector<TimerEventListener*> listeners;
public:
  Timer();
  virtual ~Timer();

  void update();

  void addEventListener(TimerEventListener *listener);

  void start(unsigned long timoutToSet);
  void stop();
  void pause();
  void resume();
  void reset();
  bool isPaused();
};

#endif