#include <Arduino.h>
#include "Timer.h"

Timer::Timer() {
    timeout = 0;
    paused = true;
    startMillis = millis();
}

Timer::~Timer() {
}

void Timer::update() {
    if (paused) {
        return;
    }

    if (millis() - startMillis >= timeout) {
        if (!listeners.empty())
            for (uint8_t i = 0;i < listeners.size();i++) {
                listeners[i]->onTime(this);
            }
        startMillis = millis();
    }
}

void Timer::addEventListener(TimerEventListener* listener) {
    listeners.push_back(listener);
}

void Timer::start(unsigned long timoutToSet) {
    timeout = timoutToSet;
    startMillis = millis();
    paused = false;
}

void Timer::stop() {
    reset();
    paused = true;
}

void Timer::pause() {
    paused = true;
}

void Timer::resume() {
    paused = false;
}

void Timer::reset() {
    startMillis = millis();
}

bool Timer::isPaused() {
    return paused;
}

unsigned long Timer::getPassedMillis() {
    return millis() - startMillis;
}