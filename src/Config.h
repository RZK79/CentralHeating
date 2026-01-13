#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>

#define FEEDER                      3
#define LIGHTER                     4
#define CO_PUMP                     5
#define CWU_PUMP                    6
#define TC_CS                       7
#define SELECT_BUTTON               8
#define NEXT_BUTTON                 9
#define BLOWER                      A2
#define BLOWER_ENCODER              2
#define CO_TEMP                     A0
#define CWU_TEMP                    A1
#define LCD_SDA                     SDA
#define LCD_SCK                     SCL

#define HOT_WATER_NTC               5000
#define CENTRAL_HEATING_NTC         10000

#define SECOND_MILL                 (uint32_t)1000
#define MINUTE_MILL                 (uint32_t)60*SECOND_MILL
#define SECOND                      (uint32_t)1
#define MINUTE                      (uint32_t)60*SECOND

#define FIRING_UP_TIME              4*MINUTE
#define STABILIZATION_TIME          3*MINUTE
#define TO_CLEANING_TIME            60*MINUTE_MILL
#define CLEANING_TIME               10*SECOND_MILL
#define PREBLOW_TIME                10*SECOND_MILL
#define PREFEED_TIME                10*SECOND_MILL

#define FIRING_UP_MAX_TEMP          80
#define FUMES_MIN_TEMP              70
#define FUMES_L_MODULATION_TEMP     110
#define FUMES_H_MODULATION_TEMP     170
#define FUMES_MAX_TEMP              210
#define MINIMAL_TEMP_FOR_CH         40
#define MINIMAL_TEMP_FOR_HW         50
#define FIRING_UP_TEMP_DIFF         10
#define SAFE_CH_TEMP                70

#endif