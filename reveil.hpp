// ESP32_horloge
// reveil.hpp
// (c) B. Froger 2020


#ifndef __REVEIL
#define __REVEIL

#define NBPARAMREVEIL 3

extern int heureReveil;
extern int minutesReveil;
extern char tblSymboleSonnerie[4];

void printAlarme();
void testReveil();
void configReveil(int configMode);
void initReveil();
bool isReveilActif();
bool isReveilSonne();
void switchReveilOnOff();

#endif