#include "Blower.h"
#include "Config.h"
#include "CurrentState.h"

#ifdef ENABLE_ENCODER
int Blower::currentRPS = 0;
int Blower::time = 0;
int Blower::currentRPM = 0;
#endif
bool on = false;

volatile uint8_t pwmValue = 0;
volatile uint8_t pwmCounter = 0;

ISR(TIMER1_COMPA_vect) {
    if (on) {
        pwmCounter++;
        if (pwmCounter < pwmValue) {
            digitalWrite(BLOWER, HIGH);
        } else {
            digitalWrite(BLOWER, LOW);
        }
    } else {
        digitalWrite(BLOWER, LOW);
    }
}

#ifdef ENABLE_ENCODER
void Blower::SpeedInterrupt() {
    Blower::currentRPS++;

    if (millis() - Blower::time > 1000) {
        Blower::time = millis();
        currentRPM = Blower::currentRPS * 60;
        Blower::currentRPS = 0;
    }
}
#endif

Blower::Blower(int PWMfreq) {
    pinMode(BLOWER, OUTPUT);
    digitalWrite(BLOWER, LOW);

#ifdef ENABLE_ENCODER
    pinMode(BLOWER_ENCODER, INPUT);
    attachInterrupt(INT0, Blower::SpeedInterrupt, RISING);
#endif

    stop();
    int freq = F_CPU / PWMfreq;

    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = (freq / 8) - 1;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS11);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

void Blower::start() {
#ifdef ENABLE_ENCODER    
    currentRPM = 0;
    currentRPS = 0;
    time = millis();
#endif
    on = true;
}

void Blower::stop() {
    on = false;
    setSpeed(0);
}

void Blower::setSpeed(int _speed) {
    float duty;
    speed = _speed;
    pwmValue = (unsigned int)((speed / 100.0f) * 255);
}

int Blower::getSpeed() {
    return speed;
}

void Blower::update() {
}

bool Blower::isOn() {
    return on;
}