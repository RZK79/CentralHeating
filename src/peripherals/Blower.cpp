#include "Blower.h"
#include "Config.h"
#include "CurrentState.h"

int Blower::currentRPS = 0;
int Blower::time = 0;
int Blower::currentRPM = 0;

void Blower::SpeedInterrupt() {
    Blower::currentRPS++;

    if (millis() - Blower::time > 1000) {
        Blower::time = millis();
        currentRPM = Blower::currentRPS * 60;
        Blower::currentRPS = 0;
    }
}

Blower::Blower() {
    stop();

    pinMode(BLOWER, OUTPUT);
    pinMode(BLOWER_ENCODER, INPUT);
    digitalWrite(BLOWER, HIGH);

    attachInterrupt(INT0, Blower::SpeedInterrupt, RISING);
}

void Blower::start() {
    currentRPM = 0;
    currentRPS = 0;
    time = millis();
}

void Blower::stop() {
    setSpeed(BlowerSpeed::RPM_0);
}

void Blower::setSpeed(BlowerSpeed _speed) {
    speed = _speed;
}

BlowerSpeed Blower::getSpeed() {
    return speed;
}

void Blower::update() {
    float duty = 0;

    switch (speed) {
        case BlowerSpeed::RPM_3600:
            duty = 100;
            break;
        case BlowerSpeed::RPM_3000:
            duty = 70.0f;
            break;
        case BlowerSpeed::RPM_2500:
            duty = 50.0f;
            break;
        case BlowerSpeed::RPM_2000:
            duty = 26.5f;
            break;
        case BlowerSpeed::RPM_1500:
            duty = 13.0f;
            break;
        case BlowerSpeed::RPM_1000:
            duty = 9.5f;
            break;

        case BlowerSpeed::RPM_0:
        default:
            duty = 0;
            break;
    }

    digitalWrite(BLOWER, HIGH);
    delayMicroseconds((unsigned int)((duty / 100) * 10000.0f));
    digitalWrite(BLOWER, LOW);
    delayMicroseconds((unsigned int)(((100.0f - duty) / 100) * 10000.0f));
}

bool Blower::isOn() {
    return speed != BlowerSpeed::RPM_0 ? true : false;
}