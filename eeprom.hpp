// ESP32_horloge
// eeprom.hpp
// (c) B. Froger 2020

#include <Arduino.h>

#ifndef __EEPROM
#define __EEPROM

#define NB_ACCES_POINTS 10

extern void initSaveToFlash(void);
extern void saveDatasToFlash(void);
extern String getSsid(int);
extern String getPwd(int);
extern void setSsid(int, String, String);
extern int isAvailableAccesPoint(String ssid);
extern char *getDefaultAccesPoint();

#endif