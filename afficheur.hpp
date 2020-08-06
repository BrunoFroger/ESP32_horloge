// ESP32_horloge
// afficheur.hpp
// (c) B. Froger 2020

#ifndef __AFFICHEUR
#define __AFFICHEUR

#include <LiquidCrystal_I2C_esp32.h>

extern LiquidCrystal_I2C lcd;


void initAfficheur();
void afficheTexte(LiquidCrystal_I2C lcd, int colonne, int ligne, String texte);
void printReglages();

#endif