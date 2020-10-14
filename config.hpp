// ESP32_horloge
// config.hpp
// (c) B. Froger 2020


#ifndef __CONFIG
#define __CONFIG

#include <LiquidCrystal_I2C.h>

#define NBDISPLAYMODE   7

extern bool configMode;
extern int cptConfigMode;
extern char tblModeDisplay[NBDISPLAYMODE][15];
extern boolean tblConfigModule[NBDISPLAYMODE];

void configInit();
boolean switchModuleStatus(int index);

#endif