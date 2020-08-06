// ESP32_horloge
// afficheur.cpp
// (c) B. Froger 2020


#include <arduino.h>
#include <LiquidCrystal_I2C_esp32.h>

#include "wifiTools.hpp"
#include "reveil.hpp"


// afficheur
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
// LiquidCrystal_I2C lcd(0x23, lcdColumns, lcdRows);  


//=========================================
//
//          initAfficheur
//
//=========================================
void initAfficheur(){
    lcd.init();
    lcd.setBacklight(HIGH); 
    lcd.clear();
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
//          printReglages
//
//=========================================
void printReglages() {
    afficheTexte(lcd, 0 ,0 ,"Reglages");
}