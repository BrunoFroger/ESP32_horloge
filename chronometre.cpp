// ESP32_horloge
// chronometre.cpp
// (c) B. Froger 2020

#include <arduino.h>

#include "afficheur.hpp"
#include "bouton.hpp"
#include "config.hpp"

bool chronoStarted = false;
char tmp[20];
char status[4] = "off";
unsigned long delayRefresh, lastRefresh;
unsigned long startValue = 0;
unsigned long currentValue = 0;
unsigned int chronoSecondes = 0;
unsigned long chronoMilliSecondes = 0;
int chronoMinutes = 0;
unsigned int chronoHeures = 0;

//=========================================
//
//          displayChrono
//
//=========================================
void displayChrono(){
    sprintf(tmp,"%04d:%02d:%02d:%03d", chronoHeures, chronoMinutes, chronoSecondes, chronoMilliSecondes);
    afficheTexte(lcd, 0, 1, tmp);
}


//=========================================
//
//          resetChrono
//
//=========================================
void resetChrono(){
   chronoHeures = 0;
   chronoMinutes = 0;
   chronoSecondes = 0;
   chronoMilliSecondes = 0;
}

int lastCptConfigMode=0;
//=========================================
//
//          chronometre
//
//=========================================
void chronometre(int cptConfigMode) {

    if (chronoStarted){
        currentValue = millis() - startValue;
        chronoMilliSecondes = currentValue % 1000;
        currentValue = currentValue / 1000;
        chronoSecondes = currentValue % 60;
        currentValue = currentValue / 60;
        chronoMinutes = currentValue % 60;
        chronoHeures = currentValue / 60;
    }

    delayRefresh = (millis() - lastRefresh);
    if (delayRefresh > 100){
        lastRefresh = millis();
        //Serial.println("refresh Chrono");
        if (chronoStarted){
            strcpy(status,"on");
        } else {
            strcpy(status,"off");
        }
        sprintf(tmp,"chrono : %s  ", status);
        afficheTexte(lcd, 0, 0, tmp);
        displayChrono();
    } else {
        if (chronoStarted){
            displayChrono();
        }
    }
    if (cptConfigMode != lastCptConfigMode){
        chronoStarted = !chronoStarted;
        if (chronoStarted){
            startValue = millis();
        }
        Serial.println("start/stop chrono");
        lastCptConfigMode = cptConfigMode;
    }
    if(isLongClic()){
        configMode = false;
        lcd.clear();
    }
}

//=========================================
//
//          printChronometre
//
//=========================================
void printChronometre() {
    afficheTexte(lcd,0,0,"chronometre");
}
