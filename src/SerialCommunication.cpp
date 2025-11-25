#include <string.h>
#include "SerialCommunication.h"
#include "CurrentState.h"
#include "Controller.h"

SerialCommunication* SerialCommunication::instance = nullptr;

SerialCommunication* SerialCommunication::get() {
    if (SerialCommunication::instance == nullptr) {
        SerialCommunication::instance = new SerialCommunication();
    }
    return SerialCommunication::instance;
}

SerialCommunication::SerialCommunication() {
    init();
}

void SerialCommunication::init() {
    recvInProgress = false;
    data[32] = { '\0' };
    i = 0;
}

void SerialCommunication::serialEvent() {
    char c;

    while (Serial.available() > 0) {
        c = Serial.read();
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

void SerialCommunication::parseData(char* data) {
    if (strstr(data, "ton") != NULL) {
        controller->getCurrentState()->isOn = true;
    } else if (strstr(data, "toff") != NULL) {
        controller->getCurrentState()->isOn = false;
    } else if (strstr(data, "sft") != NULL) {
        controller->getCurrentState()->feederTimeToSet = atoi(&data[3]);
    } else if (strstr(data, "sfp") != NULL) {
        controller->getCurrentState()->feederPeriodToSet = atoi(&data[3]);
    } else if (strstr(data, "sb") != NULL) {
        controller->getCurrentState()->blowerSpeedToSet = atoi(&data[2]);
    } else if (strstr(data, "sch") != NULL) {
        controller->getCurrentState()->centralHeatingTemperatureToSet = atoi(&data[3]);
    } else if (strstr(data, "shw") != NULL) {
        controller->getCurrentState()->hotWaterTemperatureToSet = atoi(&data[3]);
    } else if (strcmp(data, "gch") == 0) {
        Serial.print("*gch");
        Serial.print(controller->getCurrentState()->centralHeatingTemperature);
        Serial.print("#");
    } else if (strstr(data, "ghw") != NULL) {
        Serial.print("*ghw");
        Serial.print(controller->getCurrentState()->hotWaterTemperature);
        Serial.print("#");
    } else if (strstr(data, "gf") != NULL) {
        Serial.print("*gf");
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
        Serial.print("*");
        Serial.print("#");
    } else if (strcmp(data, "gb") == 0) {
        Serial.print("*");
        Serial.print(controller->getBlower()->isOn());
        Serial.print("#");
    } else if (strcmp(data, "gf") == 0) {
        Serial.print("*");
        Serial.print(controller->getFeeder()->isFeederOn());
        Serial.print("#");
    }
}
