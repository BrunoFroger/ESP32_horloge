// ESP32_horloge
// ESP32_horloge.ino
// (c) B. Froger 2020

#include <arduino.h>
//#include <WiFi.h>

#include "afficheur.hpp"
#include "bouton.hpp"
#include "config.hpp"
#include "chronometre.hpp"
#include "cptRebours.hpp"
#include "wifiTools.hpp"
#include "reveil.hpp"
#include "ntp.hpp"
#include "heure.hpp"
#include "serverWeb.hpp"
#include "eeprom.hpp"
#include "temperature.hpp"

// set the LCD parameters

// time varaibles
unsigned long nbMillis;
unsigned long delaiLastAction;
int displayMode, configHeureMode;
boolean action_en_cours;

#define NBDISPLAYMODE   7
char tblModeDisplay[NBDISPLAYMODE][15] = {"heure", "alarme", "chronometre", "cpt rebours", "Wifi", "NTP", "temperature"};

//=========================================
//
//          setup
//
//=========================================
void setup() {
    delay(1000);
    // initialize serial communication
    Serial.begin(115200);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
        break;
        delay(10);
    }
    delay(1000);
    if (timeoutInitSerial != 0)
    {
        Serial.println();
        Serial.println("Serial initialized");
    } else
    {
        Serial.println("Serial not initialized");
        exit(0);
    }

    initAfficheur();
    delay(500);
    afficheTexte(lcd,0,0,"afficheur OK");
    Serial.println("afficheur OK");
    delay(1000);
    lcd.clear();

    nbMillis = millis();
    configInit();

    displayMode = 0;

    resetChrono();
    resetHeure();
    initReveil();
    initTemperature();

    Serial.println("Initialisation de l'eeprom ...");
    restoreDatasfromFlash(true);
    Serial.println("Initialisation de l'eeprom OK");
    
    afficheTexte(lcd,0,0,"try wifi ...");
    Serial.println("try wifi ...");
    delay(1000);
    initWifi();
    delay(1000);
    afficheTexte(lcd,0,1,getSsid());
    delay(1000);

    initNtp();
    refreshNtpNow();

    lcd.clear();

    Serial.println("Fin de setup");
}

//=========================================
//
//          loop
//
//=========================================
void loop() {

   updateHeure();

    if ((millis() - nbMillis) >= 100){
        nbMillis = millis();
        switch (displayMode){
            case 0 : // mode affichage de l'heure
                if (!configMode){
                    // affichege de l'ecran d'heure
                    printHeure();
                } else {
                    // affichage des ecrans de config de l'heure
                    configHeure(cptConfigMode);
                }
                break;
            case 1 : // mode affichage heure infos aloarme
                action_en_cours = true;
                if (!configMode){
                    // affichege de l'ecran d'alarme
                    printAlarme();
                } else {
                    // affichage des ecrans de config de l'alarme
                    configReveil(cptConfigMode);
                }
                break;
            case 2 : // mode affichage chronometre
                action_en_cours = true;
                if (!configMode){
                    printChronometre();
                } else {
                    chronometre(cptConfigMode);
                }
                break;
            case 3 : // mode affichage compte a rebours
                action_en_cours = true;
                if (!configMode){
                    printCptRebours();
                } else {
                    cptRebours(cptConfigMode);
                }
                break;
            case 4 : // mode wifi
                action_en_cours = true;
                if (!configMode){
                    printInfoWifi();
                } else {
                    lcd.clear();
                    afficheTexte(lcd,0,0,"rescan Wifi");
                    initWifi();
                    refreshNtpNow();
                    configMode = false;
                }
                break;
            case 5 : // mode NTP
                action_en_cours = true;
                if (!configMode){
                    printNtp();
                } else {
                    refreshNtpNow();
                    configMode = false;
                }
                break;
            case 6 : // mode Temperature
                action_en_cours = false;
                if (!configMode){
                    printTemperature();
                    temperature(cptConfigMode);
                }
                break;
        }
    }
    if (isShortClic()){
        if (!configMode) {
            // mode affichage incremente le menu principal
            displayMode++;
        } else {
            // mode config on incremente le sous menu
            cptConfigMode++;
        }
        displayMode = displayMode % NBDISPLAYMODE;
        /*Serial.print("display mode = ");
        Serial.println(displayMode);
        Serial.print("config mode = ");
        Serial.println(cptConfigMode);*/
        lcd.clear();
    }
    if (!configMode){
        if (isLongClic()){
            configMode = true;
            cptConfigMode = 0;
        }
    }

    // check request from clients
    wifiServer.handleClient();

/*
    // Check if a client has connected
    wifiClient = wifiServer.available();
    if (wifiClient) {
        // a client is connected
        //check if the client sends some data
        // delay(50); // wait a lot of time to receive datas
        if (wifiClient.available()) {
            // a request is available treat int
            // Read the first line of the request
            String request = wifiClient.readStringUntil('\r');
            //Serial.print( "String recue du Client:   "); 
            //Serial.println(request);
            analyseRequest(request);
        }
    }
*/
    // si aucune action au bout de 10s on revient a l'ecran d'affichage de l'heure
    if (action_en_cours){
        delaiLastAction = millis() - getLastAction();
        if (delaiLastAction > 10000){
            lcd.clear();
            configMode = false;
            cptConfigMode = 0;
            displayMode = 0;
            action_en_cours = false;
        }
    }

    readBouton();
    refreshNtp();
    testReveil();
    delay (1);
}
