#ifndef _BLOWERSPEED_H_
#define _BLOWERSPEED_H_

#include <Arduino.h>

enum BlowerSpeed {
    RPM_0,
    RPM_1000,
    RPM_1500,
    RPM_2000,
    RPM_2500,
    RPM_3000,
    RPM_3600
};

extern inline int BlowerSpeedToValue(BlowerSpeed speed) {
    switch (speed) {
        case BlowerSpeed::RPM_3600:
            return 3600;

        case BlowerSpeed::RPM_3000:
            return 3000;

        case BlowerSpeed::RPM_2500:
            return 2500;

        case BlowerSpeed::RPM_2000:
            return 2000;

        case BlowerSpeed::RPM_1500:
            return 1500;

        case BlowerSpeed::RPM_1000:
            return 1000;

        case BlowerSpeed::RPM_0:
            return 0;
    }
}

extern inline const char* BlowerSpeedToString(uint8_t speed) {
    switch (speed) {
        case BlowerSpeed::RPM_3600:
            return "3600";

        case BlowerSpeed::RPM_3000:
            return "3000";

        case BlowerSpeed::RPM_2500:
            return "2500";

        case BlowerSpeed::RPM_2000:
            return "2000";

        case BlowerSpeed::RPM_1500:
            return "1500";

        case BlowerSpeed::RPM_1000:
            return "1000";

        case BlowerSpeed::RPM_0:
            return "0";
    }
}

#endif