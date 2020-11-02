// ESP32_horloge
// bouton.hpp
// (c) B. Froger 2020


#ifndef __BOUTON
#define __BOUTON


#define PIN_BOUTON 2            // D4

#define ENCODEUR_CLOCK_PIN  14  // D5
#define ENCODEUR_DT_PIN     12  // D6

extern void initBouton(void);
extern bool readBouton(void);
extern bool getBoutonState(void);
extern bool isShortClic(void);
extern bool isLongClic(void);
extern unsigned long getLastAction();
extern int getEncodeurDeltaValue();
extern int getEncodeurValue();

#endif