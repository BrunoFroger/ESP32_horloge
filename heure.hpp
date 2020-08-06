// ESP32_horloge
// heure.hpp
// (c) B. Froger 2020



#ifndef __HEURE
#define __HEURE


extern int second;
extern int minute;
extern int heure;
extern int jour;
extern int jourSemaine;
extern int mois;
extern int annee;

void updateHeure();
void printHeure();
void resetHeure();
void configHeure(int);

#endif
