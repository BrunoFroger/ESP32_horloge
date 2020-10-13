// ESP32_horloge
// wifiTools.hpp
// (c) B. Froger 2020

#ifndef __WIFITOOLS
#define __WIFITOOLS

//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

extern WiFiClient wifiClient;
extern ESP8266WebServer wifiServer;

extern void initWifi(void);
extern char *getSsid();
extern char *getIp();
extern void printInfoWifi();

#endif