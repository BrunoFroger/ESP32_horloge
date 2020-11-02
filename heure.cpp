// ESP32_horloge
// heure.cpp
// (c) B. Froger 2020

#include <arduino.h>

#include "reveil.hpp"
#include "afficheur.hpp"
#include "bouton.hpp"
#include "config.hpp"
#include "ntp.hpp"


// parametre temps
int second;
int minute;
int heure;
int jour;
int jourSemaine;
int mois;
int annee;

#define NBPARAMHEURE 6
char tblParamHeure[NBPARAMHEURE][20] = {"annee", "mois", "jour", "heure", "minutes", "secondes"};

unsigned long nbMillisAfficheHeure = 0;
unsigned long nbMillisecondUpdateHeure = 0;

//=========================================
//
//          resetHeure
//
//=========================================
void resetHeure(){
    Serial.println("Reset Heure");
    second = 00;
    minute = 00;    
    heure = 00;
    jour = 1;
    mois = 1;
    annee = 1970;
}

//=========================================
//
//          configHeure
//
//=========================================
void configHeure(int cptConfigModeHeure){
    char tmp[20];
    int valeur;
    if (cptConfigMode >= NBPARAMHEURE) {
        configMode = false;
        return;
    }
    afficheTexte(lcd, 0, 0, "Mise a l'heure");
    switch (cptConfigModeHeure){
        case 0 : valeur = annee; break;
        case 1 : valeur = mois; break;
        case 2 : valeur = jour; break;
        case 3 : valeur = heure; break;
        case 4 : valeur = minute; break;
        case 5 : valeur = second; break;
    }
    sprintf(tmp, "%s : %4d", tblParamHeure[cptConfigModeHeure], valeur);
    afficheTexte(lcd, 0, 1, tmp);
    if (isShortClic()){
        cptConfigModeHeure++;
        if (cptConfigModeHeure >= NBPARAMHEURE){
            Serial.println("On quitte le mode config");
            configMode = false;
            cptConfigModeHeure = 0;
        } else {
            Serial.print("on passe a la config heure suivante : ");
            Serial.println(cptConfigModeHeure);
        }
        lcd.clear();
    }
    int deltaValue = getEncodeurDeltaValue();
    if (deltaValue != 0){
        Serial.print("incremente la valeur de ");
        Serial.print(tblParamHeure[cptConfigModeHeure]);
        switch(cptConfigModeHeure){
            case 0 : 
                annee += deltaValue; 
                deltaValue = annee;
                break;
            case 1 : 
                mois += deltaValue; 
                deltaValue = mois;
                break;
            case 2 : 
                jour += deltaValue; 
                deltaValue = jour;
                break;
            case 3 : 
                heure += deltaValue; 
                deltaValue = heure;
                break;
            case 4 : 
                minute += deltaValue; 
                deltaValue = minute;
                break;
            case 5 : 
                second += deltaValue; 
                deltaValue = second;
                break;
        }
        Serial.print(" => ");
        Serial.println(valeur);
        setBlink(false);    // arrete le clignotement si actif
    }
    
}

//=========================================
//
//          updateHeure
//
//=========================================
void updateHeure(){
    int dernierJour = 31;

    if ((millis() - nbMillisecondUpdateHeure) >= 1000){
        nbMillisecondUpdateHeure = millis();

        second++;

        if (second >= 60){
            second = 0;
            minute++;
        }
        if (minute >= 60){
            minute = 0;
            heure++;
        }
        if (heure >= 24){
            heure = 0;
            jour++;
        }
        switch (mois){  
            case 4:
            case 6:
            case 9:
            case 11:    
                dernierJour = 30;
                break;      
            case 2:
                if ((annee % 4) != 0){
                    dernierJour = 29;
                } else {
                    dernierJour = 28;
                }
                break;
            default:
                dernierJour = 31;
        }
        if (jour > dernierJour){
            jour = 1;
            mois++;
        }
        if (mois > 12){
            mois = 0;
            annee++;
        }
    }
}

//=========================================
//
//          printHeure
//
//=========================================
void printHeure() {
    char tmp[100];
    char dateString[20];
    char heureString[20];
    char tmpChar;

    if ((millis() - nbMillisAfficheHeure) >= 100){
        nbMillisAfficheHeure = millis();
        if (isReveilActif()){
            if (isReveilSonne()){
                tmpChar = tblSymboleSonnerie[second%4];
            } else {
                //sprintf(tmp,"[*]");
                tmpChar = '*';
            }
        } else {
            //sprintf(tmp,"[ ]");
            tmpChar = ' ';
        }
        if (annee < 2000){
            setBlink(true);
        } else {
            setBlink(false);
        }
        sprintf(dateString,"%s %02d-%02d-%04d", dayOfWeek[jourSemaine], jour, mois, annee);
        sprintf(heureString,"[ ] %02d:%02d:%02d", heure, minute, second);
        afficheTexte(lcd,0,0,dateString);
        afficheTexteBlink(lcd,0,1,heureString);
        lcd.setCursor(1,1);
        lcd.write(tmpChar);
        sprintf(tmp,"%s/%s", dateString,heureString);
        //Serial.println (tmp);

    }
}
