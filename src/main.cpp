#include <Arduino.h>
#include "Controller.h"

void setup() {
    Controller::get()->setup();
}

void loop() {
    Controller::get()->loop();
}

void serialEvent(){
    Controller::get()->getSerialCommunication()->serialEvent();
}
