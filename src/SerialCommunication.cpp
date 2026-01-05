#include "SerialCommunication.h"
#include "Controller.h"
#include "CurrentState.h"
#include "Errors.h"
#include "peripherals/NTCType.h"
#include <avr/wdt.h>
#include <string.h>

SerialCommunication::SerialCommunication(){
    Serial.begin(9600);
    init();
}

void SerialCommunication::init()
{
    recvInProgress = false;
    data[0] = { '\0' };
    i = 0;
}

void SerialCommunication::serialEvent()
{
    while(Serial.available()){
        char c = Serial.read();
        if (recvInProgress) {
            if (c != end) {
                data[i++] = c;
                if (i >= 32) {
                    i = 31;
                }
            } else {
                data[i] = '\0';
                recvInProgress = false;
                i = 0;
                parseData(data);
            }
        } else if (c == start) {
            init();
            recvInProgress = true;
        }
    }
}

void SerialCommunication::parseData(char* data)
{
    if (strncmp("ton", data, 3) == 0) {
        Controller::get()->getCurrentState()->isOn = true;
    } else if (strncmp("toff", data, 4) == 0) {
        Controller::get()->getCurrentState()->isOn = false;
    } else if (strncmp("sft", data, 3) == 0) {
        Controller::get()->getCurrentState()->feederTimeToSet = atoi(&data[3]);
        Controller::get()->getFeeder()->setFeedTime(Controller::get()->getCurrentState()->feederTimeToSet);
    } else if (strncmp("sfp", data, 3) == 0) {
        Controller::get()->getCurrentState()->feederPeriodToSet = atoi(&data[3]);
        Controller::get()->getFeeder()->setPeriodTime(Controller::get()->getCurrentState()->feederPeriodToSet);
    } else if (strncmp("sbsfu", data, 5) == 0) {
        Controller::get()->getCurrentState()->blowerSpeedToSetFiringUp = atoi(&data[5]);
    } else if (strncmp("sbss", data, 4) == 0) {
        Controller::get()->getCurrentState()->blowerSpeedToSetStabilization = atoi(&data[4]);
    } else if (strncmp("sbsn", data, 4) == 0) {
        Controller::get()->getCurrentState()->blowerSpeedToSetNormal = atoi(&data[4]);
    } else if (strncmp("scht", data, 4) == 0) {
        Controller::get()->getCurrentState()->centralHeatingTemperatureToSet = atoi(&data[4]);
    } else if (strncmp("shwt", data, 4) == 0) {
        Controller::get()->getCurrentState()->hotWaterTemperatureToSet = atoi(&data[4]);
    } else if (strncmp("sntcch", data, 6) == 0) {
        Controller::get()->getCurrentState()->NTCch = atoi(&data[6]);
    } else if (strncmp("sntchw", data, 6) == 0) {
        Controller::get()->getCurrentState()->NTChw = atoi(&data[6]);
    } else if (strncmp("chpumpon", data, 8) == 0) {
        Controller::get()->getRelays()->turnCentralHeatingPumpOn();
    } else if (strncmp("chpumpoff", data, 8) == 0) {
        Controller::get()->getRelays()->turnCentralHeatingPumpOff();
    } else if (strncmp("hwpumpon", data, 8) == 0) {
        Controller::get()->getRelays()->turnHotWaterPumpOn();
    } else if (strncmp("hwpumpoff", data, 8) == 0) {
        Controller::get()->getRelays()->turnHotWaterPumpOff();
    } else if (strncmp("gcht", data, 4) == 0) {
        Serial.print("*gcht");
        Serial.print(Controller::get()->getCurrentState()->centralHeatingTemperature);
        Serial.print("#");
    } else if (strncmp("ghwt", data, 4) == 0) {
        Serial.print("*ghwt");
        Serial.print(Controller::get()->getCurrentState()->hotWaterTemperature);
        Serial.print("#");
    } else if (strncmp("gft", data, 3) == 0) {
        Serial.print("*gft");
        Serial.print(Controller::get()->getCurrentState()->fumesTemperature);
        Serial.print("#");
    } else if (strncmp("ghwp", data, 4) == 0) {
        Serial.print("*ghwp");
        Serial.print(Controller::get()->getCurrentState()->isHotWaterPumpOn);
        Serial.print("#");
    } else if (strncmp("gchp", data, 4) == 0) {
        Serial.print("*gchp");
        Serial.print(Controller::get()->getCurrentState()->isCentralHeatingPumpOn);
        Serial.print("#");
    } else if (strncmp("gl", data, 2) == 0) {
        Serial.print("*gl");
        Serial.print(Controller::get()->getCurrentState()->lighter);
        Serial.print("#");
    } else if (strncmp("gbs", data, 3) == 0) {
        Serial.print("*gbs");
        Serial.print(Controller::get()->getBlower()->getSpeed());
        Serial.print("#");
    } else if (strncmp("gf", data, 2) == 0) {
        Serial.print("*gf");
        Serial.print(Controller::get()->getFeeder()->isFeederOn());
        Serial.print("#");
    } else if (strncmp("error", data, 5) == 0) {
        Serial.print("*error");
        Serial.print(Controller::get()->getCurrentState()->error);
        Serial.print("#");
    } else if (strncmp("reseterror", data, 10) == 0) {
        Controller::get()->getCurrentState()->error = Errors::OK;
    } else if (strncmp("sfut", data, 4) == 0) {
        Controller::get()->getCurrentState()->firingUpTimeToSet = atoi(&data[4]);
    } else if (strncmp("sst", data, 3) == 0) {
        Controller::get()->getCurrentState()->stabilizationTimeToSet = atoi(&data[3]);
    } else if (strncmp("reset", data, 5) == 0) {
        wdt_disable();
        wdt_enable(WDTO_15MS);
        while (true) { }
    }
}
