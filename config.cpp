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

// other variables
bool configMode = false;
int cptConfigMode = 0;

//=========================================
//
//          config
//
//=========================================
void configInit(){
    configMode = false;
    cptConfigMode = 0;
}
