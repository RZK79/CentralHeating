#include "Relays.h"
#include "Config.h"
#include "Controller.h"
#include "CurrentState.h"

Relays::Relays() {
    pinMode(CO_PUMP, OUTPUT);
    pinMode(CWU_PUMP, OUTPUT);
    pinMode(LIGHTER, OUTPUT);

}

void Relays::turnOffAll() {
    digitalWrite(CO_PUMP, HIGH);
    digitalWrite(CWU_PUMP, HIGH);
    digitalWrite(LIGHTER, HIGH);

    controller->getCurrentState()->isCentralHeatingPumpOn = false;
    controller->getCurrentState()->isHotWaterPumpOn = false;
    controller->getCurrentState()->lighter = false;
}

void Relays::turnLighterOn() {
    controller->getCurrentState()->lighter = true;
    digitalWrite(LIGHTER, LOW);
}

void Relays::turnLighterOff() {
    controller->getCurrentState()->lighter = false;
    digitalWrite(LIGHTER, HIGH);
}

bool Relays::isCentralHeatingPumpOn() {
    return (bool)digitalRead(CO_PUMP);
}

bool Relays::isHotWaterPumpOn() {
    return (bool)digitalRead(CWU_PUMP);
}

void Relays::turnCentralHeatingPumpOn() {
    controller->getCurrentState()->isCentralHeatingPumpOn = true;
    digitalWrite(CO_PUMP, LOW);
}

void Relays::turnCentralHeatingPumpOff() {
    controller->getCurrentState()->isCentralHeatingPumpOn = false;
    digitalWrite(CO_PUMP, HIGH);
}

void Relays::turnHotWaterPumpOn() {
    controller->getCurrentState()->isHotWaterPumpOn = true;
    digitalWrite(CWU_PUMP, LOW);
}

void Relays::turnHotWaterPumpOff() {
    controller->getCurrentState()->isHotWaterPumpOn = false;
    digitalWrite(CWU_PUMP, HIGH);
}

