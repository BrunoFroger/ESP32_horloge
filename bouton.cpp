// ESP32_horloge
// bouton.cpp
// (c) B. Froger 2020


#include <arduino.h>
#include "bouton.hpp"


boolean etatBouton=false;
unsigned long startAppui = 0;
unsigned long dureeAppui = 0;
unsigned long lastAction;

// encodeur
int encodeurClockPin;
int encodeurDtPin;
int encodeurValue;
int encodeurLastValue;
int lastClock = HIGH;

//=========================================
//
//          initBouton 
//
//=========================================
void initBouton(void){
    //Serial.println("initBouton => debut");
    encodeurClockPin = ENCODEUR_CLOCK_PIN;
    encodeurDtPin = ENCODEUR_DT_PIN;
    pinMode (encodeurClockPin, INPUT_PULLUP);
    pinMode (encodeurDtPin, INPUT_PULLUP);
    pinMode (PIN_BOUTON, INPUT_PULLUP);
    /*
    Serial.print("pin Clock => ");
    Serial.println(encodeurClockPin);
    Serial.print("pin DT    => ");
    Serial.println(encodeurDtPin);
    Serial.print("pin Btn   => ");
    Serial.println(PIN_BOUTON);
    */
    encodeurValue = 0;
    encodeurLastValue = encodeurValue;
    lastClock = HIGH;
    //Serial.println("initBouton => fin");
}

//=========================================
//
//          readEncodeur
//
//=========================================
void readEncodeur(){
    int newClock = digitalRead(encodeurClockPin);
    //Serial.println("readEncodeur => debut");
    if ((lastClock == LOW) && (newClock == HIGH)){
        if (digitalRead(encodeurDtPin) == HIGH){
            //Serial.print("encodeur.cpp : readEncodeur-- => ");
            encodeurValue--;
        } else {
            //Serial.print("encodeur.cpp : readEncodeur++ => ");
            encodeurValue++;
        }
        Serial.println(encodeurValue);
    }
    lastClock = newClock;
    //Serial.println("readEncodeur => fin");
}

//=========================================
//
//          getEncodeurValue
//
//=========================================
int getEncodeurValue(){
    //readEncodeur();
    return encodeurValue;
}

//=========================================
//
//          getEncodeurDeltaValue
//
//=========================================
int deltaEncodeur;
int getEncodeurDeltaValue(){
    //Serial.println("encodeur.cpp : getEncodeurDeltaValue => debut");
    //readEncodeur();
    deltaEncodeur = encodeurValue - encodeurLastValue;
    encodeurLastValue = encodeurValue;
    //Serial.print("valeur de l'encodeur : ");
    //Serial.println(encodeurValue);
    //Serial.println("encodeur.cpp : getEncodeurDeltaValue => fin");
    return deltaEncodeur;
}

//=========================================
//
//          getBoutonState 
//
//=========================================
boolean getBoutonState(void){
    return etatBouton;
}

//=========================================
//
//          isShortClic 
//
//=========================================
boolean isShortClic(void){
    bool tmp = (dureeAppui < 300) && (dureeAppui > 0);
    if (tmp) {
        //Serial.println("is short clic");
        dureeAppui = 0;
        lastAction = millis();
    }
    return tmp;
}

//=========================================
//
//          isLongClic 
//
//=========================================
boolean isLongClic(void){
    bool tmp = (dureeAppui > 600);
    if (tmp) {
        //Serial.println("is long clic");
        dureeAppui = 0;
        lastAction = millis();
    }
    return tmp;
}

//=========================================
//
//          testAllBoutons 
//
//=========================================
void testAllBoutons(void){
    #define NB_BOUTONS  6
    int tblBouton[NB_BOUTONS]={0,12,13,14,15,16};
    Serial.println("---------------------------------");
    for (int i = 0; i < NB_BOUTONS ; i++){
        if (digitalRead(tblBouton[i]) == 0){
            Serial.print("Bouton ");
            Serial.print(i);
            Serial.print(" appuye");
            Serial.println();
        } else {
            Serial.print("Bouton ");
            Serial.print(i);
            Serial.print(" relache");
            Serial.println();
        }
    }
}

//=========================================
//
//          readBouton 
//
//=========================================
boolean readBouton(void){
    // lecture de l'encodeur rotatif
    readEncodeur();
    // TODO a mettre en commenatire une fois le bon bouton determine
    //testAllBoutons();
    boolean boutonAppuye = (digitalRead(PIN_BOUTON) == 0);
    if (boutonAppuye && !etatBouton){
        // le bouton vient d'etre appuyé
        //Serial.println("bouton appuyé");
        etatBouton = true;
        startAppui = millis();
        dureeAppui = 0;
    } else if (boutonAppuye && etatBouton){
        // le bouton est toujour appuyé
        etatBouton = true;
    } else if (!boutonAppuye && etatBouton){
        // le bouton vient d'etre relaché
        //Serial.println("bouton relaché");
        etatBouton = false;
        dureeAppui = millis() - startAppui;
        //Serial.print("duree appui = ");
        //Serial.println(dureeAppui);
    } else if (!boutonAppuye && !etatBouton){
        // bouton relaché est toujours relaché
        etatBouton = false;
    }
    return etatBouton;
}

//=========================================
//
//          getLastAction 
//
//=========================================
unsigned long getLastAction(){
    return lastAction;
}

