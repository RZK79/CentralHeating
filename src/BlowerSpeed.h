#ifndef _BLOWERSPEED_H_
#define _BLOWERSPEED_H_

#include <Arduino.h>

enum BlowerSpeed {
    RPM_0,
    RPM_10,
    RPM_20,
    RPM_30,
    RPM_40,
    RPM_50,
    RPM_60,
    RPM_70,
    RPM_80,
    RPM_90,
    RPM_100
};

extern inline int BlowerSpeedToValue(BlowerSpeed speed) {
    switch (speed) {
        case BlowerSpeed::RPM_100:
            return 100;

        case BlowerSpeed::RPM_90:
            return 90;

        case BlowerSpeed::RPM_80:
            return 80;

        case BlowerSpeed::RPM_70:
            return 70;

        case BlowerSpeed::RPM_60:
            return 60;

        case BlowerSpeed::RPM_50:
            return 50;

        case BlowerSpeed::RPM_40:
            return 40;

        case BlowerSpeed::RPM_30:
            return 30;

        case BlowerSpeed::RPM_20:
            return 20;

        case BlowerSpeed::RPM_10:
            return 10;

        default:
        case BlowerSpeed::RPM_0:
            return 0;
    }
}

extern inline const char* BlowerSpeedToString(uint8_t speed) {
    switch (speed) {
        case BlowerSpeed::RPM_100:
            return "100%";

        case BlowerSpeed::RPM_90:
            return "90%";

        case BlowerSpeed::RPM_80:
            return "80%";

        case BlowerSpeed::RPM_70:
            return "70%";

        case BlowerSpeed::RPM_60:
            return "60%";

        case BlowerSpeed::RPM_50:
            return "50%";

        case BlowerSpeed::RPM_40:
            return "40%";

        case BlowerSpeed::RPM_30:
            return "30%";

        case BlowerSpeed::RPM_20:
            return "20%";

        case BlowerSpeed::RPM_10:
            return "10%";

        default:
        case BlowerSpeed::RPM_0:
            return "0%";
    }
}

#endif