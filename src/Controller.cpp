#include "Controller.h"
#include "Config.h"
#include "Errors.h"
#include "SerialCommunication.h"
#include "peripherals/Relays.h"

static bool DBG = false;

Controller* Controller::instance = nullptr;

Controller* Controller::get()
{
    if (instance == nullptr) {
        instance = new Controller();
    }

    return instance;
}

Controller::Controller()
{
    se = new SerialCommunication();
    currentState = new CurrentState();
    currentState->setDefault();
}

void Controller::setup()
{
    state = State::OFF;

    fumesTemperature = new ThermoCouple(TC_CS);
    boilerTemperature = new NTC(NTC::ToValue(currentState->NTCch), CO_TEMP);
    hotWaterTankTemperature = new NTC(NTC::ToValue(currentState->NTChw), CWU_TEMP);

    feeder = new Feeder();
    feeder->stop();
    updateables.push_back(feeder);

    blower = new Blower(15000);
    updateables.push_back(blower);

    relays = new Relays();
    relays->turnOffAll();
    
    loopTimer = new LoopTimer();
    updateables.push_back(loopTimer);
    cleaningTimer = new CleaningTimer();
    updateables.push_back(cleaningTimer);
    currentStateTimer = new Timer();
    updateables.push_back(currentStateTimer);
    currentStateTimer->addEventListener(this);
    
    changeStateTo(State::OFF);
}

void Controller::loop()
{
    for(unsigned int i=0;i<updateables.size();i++){
        updateables[i]->update();
    }
    se->serialEvent();
}

void Controller::getSensorsData()
{
    currentState->fumesTemperature = (int)fumesTemperature->getTemperature();
    currentState->centralHeatingTemperature = (int)boilerTemperature->getTemperature();
    currentState->hotWaterTemperature = (int)hotWaterTankTemperature->getTemperature();

    if(DBG){
        Serial.println(currentState->hotWaterTemperature );
    }
}

void Controller::onTime(Timer* timer)
{
    currentStateTime++;
    
    if (state == Controller::State::NORMAL) {
        if(blower->getSpeed() != currentState->blowerSpeedToSetNormal){
            blower->setSpeed(currentState->blowerSpeedToSetNormal);
        }
    }else if(state == Controller::State::FIRING_UP){
        if(blower->getSpeed() != currentState->blowerSpeedToSetFiringUp){
            blower->setSpeed(currentState->blowerSpeedToSetFiringUp);
        }
    }else if(state == Controller::State::STABILIZATION){
        if(blower->getSpeed() != currentState->blowerSpeedToSetStabilization){
            blower->setSpeed(currentState->blowerSpeedToSetStabilization);
        }
    }

#ifdef SIMULATION
    if (state != State::OFF && state != State::EXTINCTION && currentState->fumesTemperature < 120) {
        currentState->fumesTemperature++;
    } else if (state == State::EXTINCTION && currentState->fumesTemperature >= 50) {
        currentState->fumesTemperature--;
    } else {
        if (state == Controller::State::NORMAL) {
            if (currentState->fumesTemperature == 120) {
                if (currentState->centralHeatingTemperature < currentState->centralHeatingTemperatureToSet) {
                    currentState->centralHeatingTemperature++;
                }

                if (currentState->isHotWaterPumpOn) {
                    if (currentState->hotWaterTemperature < currentState->hotWaterTemperatureToSet) {
                        currentState->hotWaterTemperature++;
                    }
                }
            }
        }
    }
#else
    getSensorsData();
#endif
}

void Controller::changeStateTo(State newState)
{
    currentStateTime = 0;
    currentStateTimer->start(1000);
    state = newState;
}

SerialCommunication* Controller::getSerialCommunication(){
    return se;
}

Controller::State Controller::getState()
{
    return state;
}

Feeder* Controller::getFeeder()
{
    return feeder;
}

Blower* Controller::getBlower()
{
    return blower;
}

LoopTimer* Controller::getLoopTimer()
{
    return loopTimer;
}

CleaningTimer* Controller::getCleaningTimer()
{
    return cleaningTimer;
}

CurrentState* Controller::getCurrentState()
{
    return currentState;
}

Relays* Controller::getRelays()
{
    return relays;
}

uint32_t Controller::getCurrentStateTime()
{
    return currentStateTime;
}

