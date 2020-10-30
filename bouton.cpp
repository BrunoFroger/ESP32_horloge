// ESP32_horloge
// bouton.cpp
// (c) B. Froger 2020


#include <arduino.h>

#define PIN_BOUTON 0   // D3     

boolean etatBouton=false;
unsigned long startAppui = 0;
unsigned long dureeAppui = 0;
unsigned long lastAction;

//=========================================
//
//          modifieVariable 
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