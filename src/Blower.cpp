#include "Blower.h"
#include "Config.h"
#include "CurrentState.h"

Blower::Blower() {
    currentRPM = 0;
    pinMode(BLOWER, OUTPUT);
}

void Blower::start() {
    CurrentState::get()->isBlowerOn = true;
}

void Blower::stop() {
    CurrentState::get()->isBlowerOn = false;
}

void Blower::setRPM(uint16_t rpm) {
    currentRPM = rpm;
}

void Blower::update() {
    if (CurrentState::get()->isBlowerOn) {
        analogWrite(BLOWER, map(currentRPM, 0, 2000, 0, 255));
    }
}