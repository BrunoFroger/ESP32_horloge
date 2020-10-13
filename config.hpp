// ESP32_horloge
// config.hpp
// (c) B. Froger 2020


#ifndef __CONFIG
#define __CONFIG

#include <LiquidCrystal_I2C.h>

extern bool configMode;
extern int cptConfigMode;

void configInit();

#endif