#include "Relays.h"
#include "Config.h"
#include "Controller.h"
#include "CurrentState.h"

void Relays::init() {
    pinMode(CO_PUMP, OUTPUT);
    pinMode(CWU_PUMP, OUTPUT);
    pinMode(LIGHTER, OUTPUT);
}

void Relays::turnOffAll() {
    digitalWrite(CO_PUMP, HIGH);
    digitalWrite(CWU_PUMP, HIGH);
    digitalWrite(LIGHTER, HIGH);

    Controller::get()->getCurrentState()->isCentralHeatingPumpOn = false;
    Controller::get()->getCurrentState()->isHotWaterPumpOn = false;
    Controller::get()->getCurrentState()->lighter = false;
}

void Relays::turnLighterOn() {
    Controller::get()->getCurrentState()->lighter = true;
    digitalWrite(LIGHTER, LOW);
}

void Relays::turnLighterOff() {
    Controller::get()->getCurrentState()->lighter = false;
    digitalWrite(LIGHTER, HIGH);
}

void Relays::turnCentralHeatingPumpOn() {
    Controller::get()->getCurrentState()->isCentralHeatingPumpOn = true;
    digitalWrite(CO_PUMP, LOW);
}

void Relays::turnCentralHeatingPumpOff() {
    Controller::get()->getCurrentState()->isCentralHeatingPumpOn = false;
    digitalWrite(CO_PUMP, HIGH);
}

void Relays::turnHotWaterPumpOn() {
    Controller::get()->getCurrentState()->isHotWaterPumpOn = true;
    digitalWrite(CWU_PUMP, LOW);
}

void Relays::turnHotWaterPumpOff() {
    Controller::get()->getCurrentState()->isHotWaterPumpOn = false;
    digitalWrite(CWU_PUMP, HIGH);
}

bool Relays::isLighterOn(){
    return (bool)digitalRead(LIGHTER);
}

bool Relays::isCentralHeatingPumpOn() {
    return (bool)digitalRead(CO_PUMP);
}

bool Relays::isHotWaterPumpOn() {
    return (bool)digitalRead(CWU_PUMP);
}
