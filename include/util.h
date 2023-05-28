#ifndef _UTIL_H_
#define _UTIL_H_

#include <Arduino.h>

#define BATTERY_PIN 34
#define PUMP_PIN 13
#define SOIL_PIN 32
#define AIR_PIN 15
#define US_TO_MIN (uint64_t) 60000000
#define TIME_TO_SLEEP (uint64_t) 15*US_TO_MIN

#endif