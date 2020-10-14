// ESP32_horloge
// config.cpp
// (c) B. Froger 2020

#include <arduino.h>
#include <string.h>

#include "afficheur.hpp"
#include "bouton.hpp"
#include "chronometre.hpp"
#include "reveil.hpp"
#include "heure.hpp"
#include "heure.hpp"
#include "eeprom.hpp"
#include "config.hpp"

// other variables
bool configMode = false;
int cptConfigMode = 0;

char tblModeDisplay[NBDISPLAYMODE][15] = {"heure", "alarme", "chronometre", "cpt rebours", "Wifi", "NTP", "temperature"};
boolean tblConfigModule[NBDISPLAYMODE];

//=========================================
//
//          switchModuleStatus
//
//=========================================
boolean switchModuleStatus(int index){
    tblConfigModule[index]= !tblConfigModule[index];
    return tblConfigModule[index];
}

//=========================================
//
//          config
//
//=========================================
void configInit(){
    configMode = false;
    cptConfigMode = 0;
    for (int i = 0 ; i < NBDISPLAYMODE ; i++) tblConfigModule[i]= true;
}
