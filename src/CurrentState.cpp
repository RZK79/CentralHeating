#include "CurrentState.h"
#include <Arduino.h>

CurrentState* CurrentState::instance = nullptr;

CurrentState* CurrentState::get() {
    if (CurrentState::instance == nullptr) {
        CurrentState::instance = new CurrentState();
    }

    return CurrentState::instance;
}

CurrentState::CurrentState(){
    
}
