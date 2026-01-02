#include "Blower.h"
#include "Config.h"
#include "CurrentState.h"

int Blower::currentRPS = 0;
int Blower::time = 0;
int Blower::currentRPM = 0;
int Blower::speed = 0;
float Blower::fullPeriod = 0.0f;

ISR(TIMER1_COMPA_vect)
{
    float duty = 0;

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
        duty = 10.0f;
        break;

    case BlowerSpeed::RPM_0:
    default:
        duty = 0;
        break;
    }

    digitalWrite(BLOWER, HIGH);
    delayMicroseconds((unsigned int)((duty / 100) * Blower::fullPeriod));
    digitalWrite(BLOWER, LOW);
    delayMicroseconds((unsigned int)(((100.0f - duty) / 100) * Blower::fullPeriod));
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

void Blower::init(int PWMfreq)
{
    stop();

    pinMode(BLOWER, OUTPUT);
    digitalWrite(BLOWER, HIGH);

#ifdef ENABLE_ENCODER
    pinMode(BLOWER_ENCODER, INPUT);
    attachInterrupt(INT0, Blower::SpeedInterrupt, RISING);
#endif

    freq = 16000000 / PWMfreq;
    fullPeriod = 1000000 / PWMfreq;

    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = freq;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10); // prescaler=1
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

void Blower::start()
{
    currentRPM = 0;
    currentRPS = 0;
    time = millis();
}

void Blower::stop()
{
    setSpeed(BlowerSpeed::RPM_0);
}

void Blower::setSpeed(int _speed)
{
    speed = _speed;
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
    return speed != BlowerSpeed::RPM_0 ? true : false;
}