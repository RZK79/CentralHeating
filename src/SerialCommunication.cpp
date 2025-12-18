#include <string.h>
#include "SerialCommunication.h"
#include "CurrentState.h"
#include "Controller.h"
#include <avr/wdt.h>
#include "peripherals/NTCType.h"
#include "Errors.h"

SerialCommunication::SerialCommunication() {
    init();
}

void SerialCommunication::init() {
    recvInProgress = false;
    data[0] = { '\0' };
    i = 0;
}

void SerialCommunication::serialEvent() {
    if (Serial.available()) {
        delay(2);
        while (Serial.available() > 0) {
            char c = Serial.read();
            if (recvInProgress) {
                if (c != SC_END) {
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
            } else if (c == SC_START) {
                init();
                recvInProgress = true;
            }
        }
    }
}

void SerialCommunication::parseData(char* data) {
    if (strncmp("ton", data, 3) == 0) {
        controller->getCurrentState()->isOn = true;
    } else if (strncmp("toff", data, 4) == 0) {
        controller->getCurrentState()->isOn = false;
    } else if (strncmp("sft", data, 3) == 0) {
        controller->getCurrentState()->feederTimeToSet = atoi(&data[3]);
    } else if (strncmp("sfp", data, 3) == 0) {
        controller->getCurrentState()->feederPeriodToSet = atoi(&data[3]);
    } else if (strncmp("sbsfu", data, 5) == 0) {
        controller->getCurrentState()->blowerSpeedToSetFiringUp = atoi(&data[5]);
    } else if (strncmp("sbss", data, 4) == 0) {
        controller->getCurrentState()->blowerSpeedToSetStabilization = atoi(&data[4]);
    } else if (strncmp("sbsn", data, 4) == 0) {
        controller->getCurrentState()->blowerSpeedToSetNormal = atoi(&data[4]);
    } else if (strncmp("scht", data, 4) == 0) {
        controller->getCurrentState()->centralHeatingTemperatureToSet = atoi(&data[4]);
    } else if (strncmp("shwt", data, 4) == 0) {
        controller->getCurrentState()->hotWaterTemperatureToSet = atoi(&data[4]);
    } else if (strncmp("sntcch", data, 6) == 0) {
        controller->getCurrentState()->NTCch = atoi(&data[6]);
    } else if (strncmp("sntchw", data, 6) == 0) {
        controller->getCurrentState()->NTChw = atoi(&data[6]);
    } else if (strncmp("gcht", data, 4) == 0) {
        Serial.print("*gcht");
        Serial.print(controller->getCurrentState()->centralHeatingTemperature);
        Serial.print("#");
    } else if (strncmp("ghwt", data, 4) == 0) {
        Serial.print("*ghwt");
        Serial.print(controller->getCurrentState()->hotWaterTemperature);
        Serial.print("#");
    } else if (strncmp("gft", data, 3) == 0) {
        Serial.print("*gft");
        Serial.print(controller->getCurrentState()->fumesTemperature);
        Serial.print("#");
    } else if (strncmp("ghwp", data, 4) == 0) {
        Serial.print("*ghwp");
        Serial.print(controller->getCurrentState()->isHotWaterPumpOn);
        Serial.print("#");
    } else if (strncmp("gchp", data, 4) == 0) {
        Serial.print("*gchp");
        Serial.print(controller->getCurrentState()->isCentralHeatingPumpOn);
        Serial.print("#");
    } else if (strncmp("gl", data, 2) == 0) {
        Serial.print("*gl");
        Serial.print(controller->getCurrentState()->lighter);
        Serial.print("#");
    } else if (strncmp("gbs", data, 3) == 0) {
        Serial.print("*gbs");
        Serial.print(controller->getBlower()->getSpeed());
        Serial.print("#");
    } else if (strncmp("gf", data, 2) == 0) {
        Serial.print("*gf");
        Serial.print(controller->getFeeder()->isFeederOn());
        Serial.print("#");
    } else if (strncmp("error", data, 5) == 0) {
        Serial.print("*error");
        Serial.print(controller->getCurrentState()->error);
        Serial.print("#");
    }else if(strncmp("reseterror", data, 10) == 0){
        controller->getCurrentState()->error = Errors::OK;
    } else if (strncmp("reset", data, 5) == 0) {
        wdt_disable();
        wdt_enable(WDTO_15MS);
        while (true) {}
    }
}
