#include "Controller.h"
#include "Config.h"
#include "Errors.h"
#include "SerialCommunication.h"
#include "peripherals/Relays.h"

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
    
    mainTimer = new MainTimer();
    updateables.push_back(mainTimer);
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

SerialCommunication* Controller::getSerialCommunication(){
    return se;
}

void Controller::getSensorsData()
{
    currentState->fumesTemperature = fumesTemperature->getTemperature();
    currentState->centralHeatingTemperature = boilerTemperature->getTemperature();
    currentState->hotWaterTemperature = hotWaterTankTemperature->getTemperature();
}

void Controller::changeStateTo(State newState)
{
    currentStateTime = 0;
    currentStateTimer->start(SECOND_MILL);
    state = newState;
}

Controller::State Controller::getState()
{
    return state;
}

const char* Controller::getStateAsString()
{
    switch (state) {
    default:
    case OFF:
        return "OFF";

    case FIRING_UP_PREBLOW:
        return "FIRING_UP_PREBLOW";

    case FIRING_UP_PREBLOW_DONE:
        return "FIRING_UP_PREBLOW_DONE";

    case PREFEED:
        return "PREFEED";

    case PREFEED_DONE:
        return "PREFEED_DONE";

    case FIRING_UP:
        return "FIRING_UP";

    case STABILIZATION:
        return "STABILIZATION";

    case NORMAL:
        return "NORMAL";

    case CLEANING:
        return "CLEANING";


    case EXTINCTION:
        return "EXTINCTION";
    }
}

Feeder* Controller::getFeeder()
{
    return feeder;
}

Blower* Controller::getBlower()
{
    return blower;
}

MainTimer* Controller::getMainTimer()
{
    return mainTimer;
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

