// ESP32_horloge
// temperature.cpp
// (c) B. Froger 2020


#include <arduino.h>
#include "DHT.h"
#include "temperature.hpp"
#include "afficheur.hpp"
#include "bouton.hpp"
#include "config.hpp"

DHT dht(DHTPIN, DHTTYPE);

unsigned long temperatureDelayRefresh, temperatureLastRefresh;

//=========================================
//
//          initTemperature
//
//=========================================
void initTemperature(){
    Serial.println("initTemperature => debut");
    dht.begin();
    temperatureLastRefresh=0;
    Serial.println("initTemperature => fin");
}

//=========================================
//
//          getTemperature
//
//=========================================
char temperatureMesure[10];
char *getTemperature(){
    float t = dht.readTemperature();
    dtostrf(t, 4, 1, temperatureMesure);
    return temperatureMesure;
}

//=========================================
//
//          getHumidity
//
//=========================================
char humiditeMesure[10];
char *getHumidity(){
    //float h = dht.readHumidity();
    float h = dht.computeHeatIndex();
    dtostrf(h, 4, 1, humiditeMesure);
    return humiditeMesure;
}

//=========================================
//
//          afficheTemperature
//
//=========================================
void afficheTemperature(){
    char tmp[20];
    /*
    char temp[10];
    char humidity[10];
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    dtostrf(t, 4, 2, temp);
    dtostrf(h, 4, 2, humidity);
    */
    //Serial.println("afficheTemperature => debut");
    /*
    Serial.print("Humidite: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.print(" *C ");
    Serial.println();
    */
    sprintf(tmp,"t=%s  h=%s", getTemperature(), getHumidity());
    //sprintf(tmp,"t=%s, h=%s", temp, humidity);
    afficheTexte(lcd,0,1,tmp);
    //Serial.println("afficheTemperature => fin");
}


//=========================================
//
//          printTemperature
//
//=========================================
void printTemperature() {
    afficheTexte(lcd,0,0,"Temperature");
}


int lastTemperatureConfigMode=0;
//=========================================
//
//          temperature
//
//=========================================
void temperature(int cptConfigMode) {
    temperatureDelayRefresh = (millis() - temperatureLastRefresh);
    if (temperatureDelayRefresh > TEMP_REFRESH){
        temperatureLastRefresh = millis();
        //Serial.println("temperature => refresh ");
        afficheTemperature();
    }



    if(isLongClic()){
        configMode = false;
        lcd.clear();
    }
}