#include "Blower.h"
#include "Config.h"
#include "CurrentState.h"

int Blower::currentRPS = 0;
int Blower::time = 0;
int Blower::currentRPM = 0;
int Blower::speed = 0;
bool Blower::on = false;
unsigned int Blower::pwmOn = 0;

ISR(TIMER1_COMPA_vect)
{
    if (Blower::on) {
        digitalWrite(BLOWER, HIGH);
        delayMicroseconds(Blower::pwmOn);
        digitalWrite(BLOWER, LOW);
    } else {
        digitalWrite(BLOWER, LOW);
    }
}

#ifdef ENABLE_ENCODER
void Blower::SpeedInterrupt()
{
    Blower::currentRPS++;

    if (millis() - Blower::time > 1000) {
        Blower::time = millis();
        currentRPM = Blower::currentRPS * 60;
        Blower::currentRPS = 0;
    }
}
#endif

Blower::Blower(int PWMfreq)
{
    pinMode(BLOWER, OUTPUT);
    digitalWrite(BLOWER, LOW);

#ifdef ENABLE_ENCODER
    pinMode(BLOWER_ENCODER, INPUT);
    attachInterrupt(INT0, Blower::SpeedInterrupt, RISING);
#endif

    stop();
    int freq = (F_CPU / PWMfreq) - 1;
    fullPeriod = clockCyclesToMicroseconds(freq);

    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = freq;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

void Blower::start()
{
    currentRPM = 0;
    currentRPS = 0;
    on = true;
#ifdef ENABLE_ENCODER
    time = millis();
#endif
}

void Blower::stop()
{
    on = false;
    setSpeed(BlowerSpeed::RPM_0);
}

void Blower::setSpeed(int _speed)
{
    float duty;
    speed = _speed;
    switch (Blower::speed) {
        case BlowerSpeed::RPM_100:
            duty = 100;
            break;
        case BlowerSpeed::RPM_90:
            duty = 90.0f;
            break;
        case BlowerSpeed::RPM_80:
            duty = 80.0f;
            break;
        case BlowerSpeed::RPM_70:
            duty = 70.0f;
            break;
        case BlowerSpeed::RPM_60:
            duty = 60.0f;
            break;
        case BlowerSpeed::RPM_50:
            duty = 50.0f;
            break;
        case BlowerSpeed::RPM_40:
            duty = 40.0f;
            break;
        case BlowerSpeed::RPM_30:
            duty = 30.0f;
            break;
        case BlowerSpeed::RPM_20:
            duty = 20.0f;
            break;
        case BlowerSpeed::RPM_10:
            duty = 1.0f;
            break;

        case BlowerSpeed::RPM_0:
        default:
            duty = 0;
            break;
    }

    pwmOn = (unsigned int)((duty / 100.0f) * fullPeriod);
}

int Blower::getSpeed()
{
    return speed;
}

void Blower::update()
{
}

bool Blower::isOn()
{
    return on;
}