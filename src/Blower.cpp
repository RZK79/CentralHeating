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
    CurrentState::get()->isBlowerOn = true;
    currentRPM = 0;
    currentRPS = 0;
    time = millis();
}

void Blower::stop() {
    CurrentState::get()->isBlowerOn = false;
    setSpeed(Speed::RPM_0);
}

void Blower::setSpeed(Speed _speed) {
    speed = _speed;
}

Blower::Speed Blower::getSpeed() {
    return speed;
}

void Blower::update() {
    if (CurrentState::get()->isBlowerOn) {
        float duty = 0;

        switch (speed) {
            case Blower::Speed::RPM_3600:
                duty = 100;
                break;
            case Blower::Speed::RPM_3000:
                duty = 70.0f;
                break;
            case Blower::Speed::RPM_2500:
                duty = 50.0f;
                break;
            case Blower::Speed::RPM_2000:
                duty = 26.5f;
                break;
            case Blower::Speed::RPM_1500:
                duty = 13.0f;
                break;
            case Blower::Speed::RPM_1000:
                duty = 9.5f;
                break;

            case Blower::Speed::RPM_0:
            default:
                duty = 0;
                break;
        }

        digitalWrite(BLOWER, HIGH);
        delayMicroseconds((unsigned int)((duty / 100) * 10000.0f));
        digitalWrite(BLOWER, LOW);
        delayMicroseconds((unsigned int)(((100.0f - duty) / 100) * 10000.0f));
    }else{
        digitalWrite(BLOWER, LOW);
    }
}