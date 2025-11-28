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
    setSpeed(Speed::RPM_0);
}

void Blower::setSpeed(Speed _speed) {
    speed = _speed;
}

Blower::Speed Blower::getSpeed() {
    return speed;
}

const char* Blower::getSpeedAsString(){
    switch(speed){
        default:
        case Blower::Speed::RPM_0:
            return "RPM_0";

        case Blower::Speed::RPM_1000:
            return "RPM_1000";

        case Blower::Speed::RPM_1500:
            return "RPM_1500";

        case Blower::Speed::RPM_2000:
            return "RPM_2000";

        case Blower::Speed::RPM_2500:
            return "RPM_2500";

        case Blower::Speed::RPM_3000:
            return "RPM_3000";

        case Blower::Speed::RPM_3600:
            return "RPM_3600";
    }
}

void Blower::update() {
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
}

bool Blower::isOn() {
    return speed != Blower::Speed::RPM_0 ? true : false;
}