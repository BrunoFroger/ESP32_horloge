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

#define NBPARAMCONFIG 4

// other variables
bool configMode = false;
int cptConfigMode = 0;
//int subConfigSelected = 0;
//char tblParamConfig[NBPARAMCONFIG][20] = {"reset all", "mise a l'heure", "chronometre", "reveil"};



//=========================================
//
//          config
//
//=========================================
void configInit(){
    configMode = false;
    cptConfigMode = 0;
}



//=========================================
//
//          config
//
//=========================================
/*
void config(){
    char tmp[20];

    switch (subConfigSelected){
        case 0: 
            afficheTexte(lcd, 0, 0, "Menu config");

            sprintf(tmp, "%s", tblParamConfig[cptConfigMode]);
            afficheTexte(lcd, 0, 1, tmp);
            if (isShortClic()){
                cptConfigMode++;
                if (cptConfigMode >= NBPARAMCONFIG){
                    Serial.println("On quitte le mode config");
                    configMode = false;
                    cptConfigMode = 0;
                    subConfigSelected = 0;
                } else {
                    Serial.print("on passe a la config suivante : ");
                    Serial.println(cptConfigMode);
                }
                lcd.clear();
            }

            if(isLongClic()){
                if (cptConfigMode == 0) {
                    resetHeure();
                    configMode = false;
                    cptConfigMode = 0;
                    subConfigSelected = 0;
                } else {
                    subConfigSelected = cptConfigMode;
                }
                lcd.clear();
            }
            break;
        case 1 : // config heure  
            configHeure(cptConfigMode);
            break;
        case 2 : // chronometre  
            chronometre();
            if(isLongClic()){
                configMode = false;
                resetChrono();
                lcd.clear();
            }
            break;
        case 3 : // chronometre  
            configReveil(cptConfigMode);
            if(isLongClic()){
                configMode = false;
                cptConfigMode = 0;
                subConfigSelected = 0;
                lcd.clear();
            }
            break;
    }
}*/