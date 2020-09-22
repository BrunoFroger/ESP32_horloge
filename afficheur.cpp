// ESP32_horloge
// afficheur.cpp
// (c) B. Froger 2020


#include <arduino.h>
#include <LiquidCrystal_I2C.h>

#include "wifiTools.hpp"
#include "reveil.hpp"


// afficheur
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x23, lcdColumns, lcdRows);
// LiquidCrystal_I2C lcd(0x23, lcdColumns, lcdRows);  
boolean blink = false;
boolean modeBlink = true;
unsigned long nbMillisBlink = 0;

//=========================================
//
//          initAfficheur
//
//=========================================
void initAfficheur(){
    lcd.init();
    lcd.setBacklight(HIGH); 
    lcd.clear();
    blink = false;
    modeBlink = true;
}

//=========================================
//
//          afficheTexte 
//
//=========================================
void afficheTexte(LiquidCrystal_I2C lcd, int colonne, int ligne, String texte){
    lcd.setCursor(colonne,ligne);
    //Serial.print("Afficheur : ");
    //Serial.println(texte);
    lcd.print(texte);
}

//=========================================
//
//          afficheTexteBlink
//
//=========================================
void afficheTexteBlink(LiquidCrystal_I2C lcd, int colonne, int ligne, String texte){
    if (blink){
        if ((millis() - nbMillisBlink) > 500){
            nbMillisBlink = millis();
            modeBlink = !modeBlink;
        }
    } else {
        modeBlink = true;
    }
    if (modeBlink){
        afficheTexte(lcd, colonne, ligne, texte);
    } else {
        afficheTexte(lcd, colonne, ligne, "                ");
    }
}

//=========================================
//
//          setBlink
//
//=========================================
void setBlink(boolean value) {
    blink = value;
 }

