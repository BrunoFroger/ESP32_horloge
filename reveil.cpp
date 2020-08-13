// ESP32_horloge
// reveil.cpp
// (c) B. Froger 2020


#include <arduino.h>

#include "afficheur.hpp"
#include "bouton.hpp"
#include "config.hpp"
#include "heure.hpp"
#include "reveil.hpp"

int heureReveil = 0;
int minutesReveil = 0;
int tmpHeureReveil, tmpMinutesReveil;
bool reveilActif = false;
bool reveilSonne = false;
unsigned long delayRefreshReveil, lastRefreshReveil;

byte antiSlash[8] = { // déclaration d’un tableau de 8 octets
  B00000, // définition de chaque octet au format binaire
  B10000, // 1 pour pixel affiché – 0 pour pixel éteint
  B01000, // les 3 bits de poids forts ne sont pas écrits car inutiles
  B00100,
  B00010,
  B00001,
  B00000,
};
int newChar = 0;
char tblSymboleSonnerie[4]; 


//=========================================
//
//          resetReveil
//
//=========================================
void resetReveil(void){
    heureReveil = 8;
    minutesReveil = 0;
    reveilActif = false;
    reveilSonne = false;
}

//=========================================
//
//          inittReveil
//
//=========================================
void initReveil(void){
    lcd.createChar(newChar,antiSlash);
    tblSymboleSonnerie[0] = '|';
    tblSymboleSonnerie[1] = '/';
    tblSymboleSonnerie[2] = '-';
    tblSymboleSonnerie[3] = newChar;
    resetReveil();
}

//=========================================
//
//          configReveil
//
//=========================================
void configReveil(int cptReveilConfig){
    char tmp[20];

    if (cptReveilConfig >= 3){
        configMode = false;
        return;
    }
    delayRefreshReveil = (millis() - lastRefreshReveil);
    if (delayRefreshReveil > 250){
        lcd.clear();
        //Serial.println("config Reveil");
        lastRefreshReveil = millis();
        afficheTexte(lcd, 0, 0, "Fonction Reveil");
        switch (cptReveilConfig){
            case 0:
                if (reveilActif){
                    sprintf(tmp,"Reveil : ON");
                } else {
                    sprintf(tmp,"Reveil : OFF");
                }
                afficheTexte(lcd, 0, 1, tmp);
                break;
            case 1:
                sprintf(tmp,"heure : %02d", heureReveil);
                afficheTexte(lcd, 0, 1, tmp);
                break;
            case 2:
                sprintf(tmp,"minutes : %02d", minutesReveil);
                afficheTexte(lcd, 0, 1, tmp);
                break;
        }
    }
    /*
    if (isShortClic()){
        cptReveilConfig++;
        if (cptReveilConfig >= NBPARAMREVEIL){
            configMode = false;
            cptReveilConfig = 0;
            tmpHeureReveil = heureReveil;
            tmpMinutesReveil = minutesReveil;
            lcd.clear();
        }
    }*/
    if (isLongClic()){
        Serial.println("long clic dans config reveil");
        switch(cptReveilConfig){
            case 0:
                reveilActif = !reveilActif;
                break;
            case 1:
                heureReveil++;
                break;
            case 2:
                minutesReveil++;
                break;
        }
    }
}

//=========================================
//
//          isReveilActif
//
//=========================================
bool isReveilActif(void){
    return reveilActif;
}

//=========================================
//
//          reveilSonne
//
//=========================================
bool isReveilSonne(void){
    return reveilSonne;
}

//=========================================
//
//          switchReveilOnOff
//
//=========================================
void switchReveilOnOff(void){
    reveilActif = !reveilActif;
    Serial.print("switch reveil on/off => ");
    /*if (reveilActif){
        Serial.println("ON ");
    } else {
        Serial.println("OFF");
    }*/
}

//=========================================
//
//          testReveil
//
//=========================================
void testReveil(void){
    if (!reveilSonne){
        if(reveilActif){
            bool tmp;
            tmp = (heureReveil == heure);
            tmp = tmp && (minutesReveil == minute);
            if (tmp) {
                Serial.println("c'est l'heure du reveil");
                reveilSonne = true;
            }
        }
    } else {
        if (reveilActif){
            // le reveil sonne on test appui sur bouton 
            // pour eteindre la sonnerie
            if (isShortClic()){
                // on eteint le reveil
                Serial.println("on eteint le reveil");
                reveilSonne = false;
                reveilActif = false;
            }
        }
    }
}

//=========================================
//
//          printAlarme
//
//=========================================
void printAlarme() {
    char tmp[100];
    if (isReveilActif()){
        afficheTexte(lcd, 0 ,0 ,"Alarme : ON ");
    } else {
        afficheTexte(lcd, 0 ,0 ,"Alarme : OFF");
    }
    sprintf(tmp,"   %02d:%02d", heureReveil, minutesReveil);
    afficheTexte(lcd, 0, 1, tmp);
}
