#include <Arduino.h>
#include "Controller.h"

void setup() {
    Serial.begin(9600);
    controller = new Controller();
}

void loop() {
    controller->loop();
}
