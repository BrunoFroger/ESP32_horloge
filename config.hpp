// ESP32_horloge
// config.hpp
// (c) B. Froger 2020


#ifndef __CONFIG
#define __CONFIG

#include <LiquidCrystal_I2C_esp32.h>

extern bool configMode;
extern int cptConfigMode;

void config();
void configInit();

#endif