// ESP32_horloge
// temperature.hpp
// (c) B. Froger 2020


#ifndef __TEMPERATURE
#define __TEMPERATURE

#define DHTPIN  13
#define DHTTYPE  DHT11

#define TEMP_REFRESH    1000

void initTemperature();
void afficheTemperature();
void printTemperature();
void temperature(int cptConfigMode);
char *getTemperature();
char *getHumidity();

#endif
