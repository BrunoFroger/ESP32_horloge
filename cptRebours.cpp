// ESP32_horloge
// cptRebours.cpp
// (c) B. Froger 2020

#include <arduino.h>

#include "afficheur.hpp"
#include "bouton.hpp"
#include "config.hpp"

bool cptReboursStarted = false;
char ReboursTmp[20];
char reboursStatus[4] = "off";
unsigned long reboursDelayRefresh, reboursLastRefresh;
unsigned long reboursStartValue = 0;
unsigned long reboursCurrentValue = 0;
unsigned int compteurSecondes = 0;
unsigned int compteurMinutes = 0;
unsigned int compteurHeures = 0;

//=========================================
//
//          displayCptRebours
//
//=========================================
void displayCptRebours(){
    sprintf(ReboursTmp,"%04d:%02d:%02d", compteurHeures, compteurMinutes, compteurSecondes);
    afficheTexte(lcd, 0, 1, ReboursTmp);
}


//=========================================
//
//          resetRebours
//
//=========================================
void resetRebours(){
   compteurHeures = 0;
   compteurMinutes = 0;
   compteurSecondes = 0;
}

int lastReboursConfigMode=0;
//=========================================
//
//          cptRebours
//
//=========================================
void cptRebours(int cptConfigMode) {

    if (cptReboursStarted){
        reboursCurrentValue = millis() - reboursStartValue;
        reboursCurrentValue = reboursCurrentValue / 1000;
        compteurSecondes = reboursCurrentValue % 60;
        reboursCurrentValue = reboursCurrentValue / 60;
        compteurMinutes = reboursCurrentValue % 60;
        compteurHeures = reboursCurrentValue / 60;
    }

    reboursDelayRefresh = (millis() - reboursLastRefresh);
    if (reboursDelayRefresh > 100){
        reboursLastRefresh = millis();
        //Serial.println("refresh Chrono");
        if (cptReboursStarted){
            strcpy(reboursStatus,"on");
        } else {
            strcpy(reboursStatus,"off");
        }
        sprintf(ReboursTmp,"cpt rebours: %s  ", reboursStatus);
        afficheTexte(lcd, 0, 0, ReboursTmp);
        displayCptRebours();
    } else {
        if (cptReboursStarted){
            displayCptRebours();
        }
    }
    if (cptConfigMode != lastReboursConfigMode){
        cptReboursStarted = !cptReboursStarted;
        if (cptReboursStarted){
            reboursStartValue = millis();
        }
        Serial.println("start/stop cpt");
        lastReboursConfigMode = cptConfigMode;
    }
    if(isLongClic()){
        configMode = false;
        lcd.clear();
    }
}

//=========================================
//
//          printCptRebours
//
//=========================================
void printCptRebours() {
    afficheTexte(lcd,0,0,"Cpt a rebours");
}
