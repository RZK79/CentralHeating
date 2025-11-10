#include <string.h>
#include "SerialCommunication.h"
#include "CurrentState.h"

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
        CurrentState::get()->isOn = true;
    } else if (strstr(data, "toff") != NULL) {
        CurrentState::get()->isOn = false;
    } else if (strstr(data, "sft") != NULL) {
        CurrentState::get()->feederTimeToSet = atoi(&data[3]);
    } else if (strstr(data, "sfp") != NULL) {
        CurrentState::get()->feederPeriodToSet = atoi(&data[3]);
    } else if (strstr(data, "sb") != NULL) {
        CurrentState::get()->blowerSpeedToSet = atoi(&data[2]);
    } else if (strstr(data, "sch") != NULL) {
        CurrentState::get()->centralHeatingTemperatureToSet = atoi(&data[3]);
    } else if (strstr(data, "shw") != NULL) {
        CurrentState::get()->hotWaterTemperatureToSet = atoi(&data[3]);
    } else if (strstr(data, "gch") != NULL) {
        Serial.print("*gch");
        Serial.print(CurrentState::get()->centralHeatingTemperature);
        Serial.print("#");
    } else if (strstr(data, "ghw") != NULL) {
        Serial.print("*ghw");
        Serial.print(CurrentState::get()->hotWaterTemperature);
        Serial.print("#");
    } else if (strstr(data, "gf") != NULL) {
        Serial.print("*gf");
        Serial.print(CurrentState::get()->fumesTemperature);
        Serial.print("#");
    }
}

