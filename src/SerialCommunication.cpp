#include <string.h>
#include "SerialCommunication.h"
#include "CurrentState.h"
#include "Controller.h"

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
    Serial.println(data);
    if (strcmp(data, "ton") == 0) {
        controller->getCurrentState()->isOn = true;
    } else if (strcmp(data, "toff") == 0) {
        controller->getCurrentState()->isOn = false;
    } else if (strcmp(data, "sft") == 0) {
        controller->getCurrentState()->feederTimeToSet = atoi(&data[3]);
    } else if (strcmp(data, "sfp") == 0) {
        controller->getCurrentState()->feederPeriodToSet = atoi(&data[3]);
    } else if (strcmp(data, "sb") == 0) {
        controller->getCurrentState()->blowerSpeedToSet = atoi(&data[2]);
    } else if (strcmp(data, "sch") == 0) {
        controller->getCurrentState()->centralHeatingTemperatureToSet = atoi(&data[3]);
    } else if (strcmp(data, "shw") == 0) {
        controller->getCurrentState()->hotWaterTemperatureToSet = atoi(&data[3]);
    } else if (strcmp(data, "gch") == 0) {
        Serial.print("*gch");
        Serial.print(controller->getCurrentState()->centralHeatingTemperature);
        Serial.print("#");
    } else if (strstr(data, "ghw") != NULL) {
        Serial.print("*ghw");
        Serial.print(controller->getCurrentState()->hotWaterTemperature);
        Serial.print("#");
    } else if (strstr(data, "gfu") != NULL) {
        Serial.print("*gfu");
        Serial.print(controller->getCurrentState()->fumesTemperature);
        Serial.print("#");
    } else if (strstr(data, "gs") != NULL) {
        Serial.print("*gs");
        Serial.print(controller->getState());
        Serial.print("#");
    } else if (strstr(data, "ghwp") != NULL) {
        Serial.print("*ghwp");
        Serial.print(controller->getCurrentState()->isHotWaterPumpOn);
        Serial.print("#");
    } else if (strcmp(data, "gchp") == 0) {
        Serial.print("*gchp");
        Serial.print(controller->getCurrentState()->isCentralHeatingPumpOn);
        Serial.print("#");
    } else if (strcmp(data, "gl") == 0) {
        Serial.print("*gl");
        Serial.print(controller->getCurrentState()->lighter);
        Serial.print("#");
    } else if (strcmp(data, "gb") == 0) {
        Serial.print("*gb");
        Serial.print(controller->getBlower()->isOn());
        Serial.print("#");
    } else if (strcmp(data, "gf") == 0) {
        Serial.print("*gf");
        Serial.print(controller->getFeeder()->isFeederOn());
        Serial.print("#");
    } else if (strcmp(data, "error") == 0) {
        Serial.print("*error");
        Serial.print(controller->getCurrentState()->error);
        Serial.print("#");
    }
}
