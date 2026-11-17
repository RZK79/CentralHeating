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
#define TO_CLEANING_TIME            60l*60l*1000l

#endif