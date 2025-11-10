#include "Relays.h"
#include "Config.h"
#include "CurrentState.h"

Relays* Relays::instance = nullptr;

Relays* Relays::get() {
    if (Relays::instance == nullptr) {
        Relays::instance = new Relays();
    }

    return Relays::instance;
}

Relays::Relays() {
    pinMode(CO_PUMP, OUTPUT);
    pinMode(CWU_PUMP, OUTPUT);
    pinMode(LIGHTER, OUTPUT);

}

void Relays::turnOffAll(){
    digitalWrite(CO_PUMP, HIGH);
    digitalWrite(CWU_PUMP, HIGH);
    digitalWrite(LIGHTER, HIGH);
    
    CurrentState::get()->isCentralHeatingPumpOn = false;
    CurrentState::get()->isHotWaterPumpOn = false;
    CurrentState::get()->lighter = false;
}

void Relays::turnLighterOn() {
    CurrentState::get()->lighter = true;
    digitalWrite(LIGHTER, LOW);
}

void Relays::turnLighterOff() {
    CurrentState::get()->lighter = false;
    digitalWrite(LIGHTER, HIGH);
}


void Relays::turnCentralHeatingPumpOn() {
    CurrentState::get()->isCentralHeatingPumpOn = true;
    digitalWrite(CO_PUMP, LOW);
}

void Relays::turnCentralHeatingPumpOff() {
    CurrentState::get()->isCentralHeatingPumpOn = false;
    digitalWrite(CO_PUMP, HIGH);
}

void Relays::turnHotWaterPumpOn() {
    CurrentState::get()->isHotWaterPumpOn = true;
    digitalWrite(CWU_PUMP, LOW);
}

void Relays::turnHotWaterPumpOff() {
    CurrentState::get()->isHotWaterPumpOn = false;
    digitalWrite(CWU_PUMP, HIGH);
}

