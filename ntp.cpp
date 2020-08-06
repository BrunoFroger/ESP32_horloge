// ESP32_horloge
// ntp.cpp
// (c) B. Froger 2020

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "heure.hpp"
#include "afficheur.hpp"
#include "bouton.hpp"
#include "config.hpp"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600 * 2, 60000);
char dayOfWeek[7][5] = {"dim", "lun", "mar", "mer", "jeu", "ven", "sam"};

//=========================================
//
//          initNtp
//
//=========================================
void initNtp(void){
    timeClient.begin();
}


#define DELAY_REFRESH_NTP   30000000
unsigned long ntpLastRefresh=-DELAY_REFRESH_NTP;
//=========================================
//
//          refreshNtpNow
//
//=========================================
void refreshNtpNow(void){
        //Serial.println("Refresh NTP");
        char tmp[50];
        timeClient.update();
        //unsigned long epochTime = timeClient.getEpochTime();
        String formatedDate = timeClient.getFormattedDate();
        //Serial.println(formatedDate);
        second = timeClient.getSeconds();
        minute = timeClient.getMinutes();
        heure = timeClient.getHours();
        jourSemaine = timeClient.getDay();
        //Serial.println(formatedDate.substring(8,10));
        jour = formatedDate.substring(8,10).toInt();
        //Serial.println(formatedDate.substring(5,7));
        mois = formatedDate.substring(5,7).toInt();
        //Serial.println(formatedDate.substring(0,4));
        annee = formatedDate.substring(0,4).toInt();
        sprintf(tmp,"Update NTP => %s %4d-%02d-%02d / %02d:%02d:%02d", dayOfWeek[jourSemaine], annee, mois, jour, heure, minute, second);
        Serial.println(tmp);
        ntpLastRefresh=millis();
}

//=========================================
//
//          refreshNtp
//
//=========================================
void refreshNtp(void){
    if ((millis() - ntpLastRefresh) > DELAY_REFRESH_NTP){
        refreshNtpNow();
    }
}

//=========================================
//
//          printNtp
//
//=========================================
void printNtp(void){
    afficheTexte(lcd,0,0,"mise a jour heure");
    afficheTexte(lcd,0,1,"clic long");
}
