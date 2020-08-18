// ESP32_horloge
// bouton.hpp
// (c) B. Froger 2020


#ifndef __BOUTON
#define __BOUTON

extern bool readBouton(void);
extern bool getBoutonState(void);
extern bool isShortClic(void);
extern bool isLongClic(void);
extern unsigned long getLastAction();

#endif