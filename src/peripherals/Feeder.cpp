#include "Feeder.h"
#include "Timer.h"
#include "Config.h"

Feeder::Feeder() {
    feederOn = false;
    
    feederTimer = new Timer();
    feederTimer->addEventListener(this);

    pinMode(FEEDER, OUTPUT);
    digitalWrite(FEEDER, HIGH);
}

void Feeder::update() {
    feederTimer->update();
}

void Feeder::prefeed() {
    feederOn = true;
    currentState = Feeder::State::PREFEED;
    digitalWrite(FEEDER, LOW);
}

void Feeder::setFeedTime(uint16_t feed) {
    feedTime = feed;
}

void Feeder::setPeriodTime(uint16_t period) {
    periodTime = period;
}

void Feeder::start() {
    feederTimer->start(feedTime);
    currentState = Feeder::State::FEED;
}

void Feeder::stop() {
    feederTimer->pause();
}

Feeder::State Feeder::getState() {
    return currentState;
}

void Feeder::onTime(Timer* timer) {
    switch (currentState) {
        case Feeder::State::FEED:
            feederOn = true;
            digitalWrite(FEEDER, LOW);
            feederTimer->start(periodTime);
            currentState = Feeder::State::PERIOD;
            break;

        case Feeder::State::PERIOD:
            feederOn = false;
            digitalWrite(FEEDER, HIGH);
            feederTimer->start(feedTime);
            currentState = Feeder::State::FEED;
            break;

        case Feeder::State::PREFEED:
            feederOn = true;
            digitalWrite(FEEDER, LOW);
            break;

        case Feeder::State::OFF:
            feederOn = false;
            break;
    }
}

bool Feeder::isFeederOn() {
    return feederOn;
}
