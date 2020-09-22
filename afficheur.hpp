// ESP32_horloge
// afficheur.hpp
// (c) B. Froger 2020

#ifndef __AFFICHEUR
#define __AFFICHEUR

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;


extern void initAfficheur();
extern void afficheTexte(LiquidCrystal_I2C lcd, int colonne, int ligne, String texte);
extern void afficheTexteBlink(LiquidCrystal_I2C lcd, int colonne, int ligne, String texte);
extern void setBlink(boolean);

#endif