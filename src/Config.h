#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>

#define FEEDER              3
#define LIGHTER             4
#define CO_PUMP             5
#define CWU_PUMP            6
#define TC_CS               7
#define SELECT_BUTTON       8
#define NEXT_BUTTON         9
#define BLOWER              A2
#define BLOWER_ENCODER      2
#define CO_TEMP             A0
#define CWU_TEMP            A1
#define LCD_SDA             SDA
#define LCD_SCK             SCL

#define HOT_WATER_NTC       5000
#define CENTRAL_HEATING_NTC 10000

#define SECOND              (uint16_t)1000
#define MINUTE              (uint16_t)60*SECOND

#define FIRING_UP_TIME      3*MINUTE
#define STABILIZATION_TIME  3*MINUTE
#define TO_CLEANING_TIME    60*MINUTE
#define CLEANING_TIME       10*SECOND
#define PREBLOW_TIME        5*SECOND
#define PREFEED_TIME        5*SECOND

#define FIRING_UP_MAX_TEMP  80
#define FIRING_UP_TEMP_DIFF 10

#endif