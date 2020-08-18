// ESP32_horloge
// wifiTools.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <Wifi.h>
#include <string.h>

#include "afficheur.hpp"
#include "eeprom.hpp"

char ipAdress[20];
char wifiSsid[25];
char wifiPassword[50];
int cptTryWifi = 0;
boolean wifiConnected = false;
boolean wifiFound = false;
WiFiClient wifiClient;
WiFiServer wifiServer(80);//Ecouter le port 80

//=========================================
//
//          scanNetworks
//
//=========================================
void deconnecteWifi(){
    WiFi.disconnect();
}

//=========================================
//
//          scanNetworks
//
//=========================================
void scanNetworks(void){    // search for availables Wifi Networks
    int nbSsid = WiFi.scanNetworks();
    if (nbSsid != -1){
        Serial.print(nbSsid);
        Serial.println(" wifi networks found");
        strcpy(wifiSsid,"");
        while (strcmp(wifiSsid,"") == 0){        
            for (int ssidNetwork = 0 ; ssidNetwork < nbSsid ; ssidNetwork++){
                Serial.print("  check network : ");
                Serial.print(WiFi.SSID(ssidNetwork));
                int availableSsidIndex = isAvailableAccesPoint(WiFi.SSID(ssidNetwork));
                if (availableSsidIndex != -1){
                    char tmp[30];
                    getSsid(availableSsidIndex).toCharArray(tmp,25);
                    //Serial.print(" on a trouve : ");
                    strcpy(wifiSsid,tmp);
                    //Serial.print(wifiSsid);
                    //Serial.print("/");
                    getPwd(availableSsidIndex).toCharArray(tmp,30);
                    strcpy(wifiPassword,tmp);
                    //Serial.print(wifiPassword);
                    Serial.println(" => OK");
                    wifiFound = true;
                    break;
                }
                Serial.println(" => NOK");
            }
            if (strcmp(wifiSsid,"") == 0){
                delay(2000);
                cptTryWifi++;
                Serial.printf("Essai %d de connexion Wifi\n",cptTryWifi);
                if (cptTryWifi > 2){
                    Serial.println("Aucun réseau détecté .. on passe");
                    wifiFound = false;
                    break;
                }
                Serial.println("No Wifi network found ==> rescan ......");
                nbSsid = WiFi.scanNetworks();
            }
        }
    }
}

//=========================================
//
//          printInfoWifi
//
//=========================================
void printInfoWifi(){
    char tmp[50];
    sprintf(tmp,"%s", wifiSsid);
    afficheTexte(lcd, 0, 0, tmp);
    sprintf(tmp,"%s", ipAdress);
    afficheTexte(lcd, 0, 1, tmp);
}

//=========================================
//
//          initWifi
//
//=========================================
void initWifi(void){    // init wifi connection

    cptTryWifi = 0;
    deconnecteWifi();
    delay(1000);
    scanNetworks();
    if (wifiFound){
        // Connect to WiFi network
        Serial.print("Connecting to ");
        Serial.println(wifiSsid);
        WiFi.begin(wifiSsid, wifiPassword);
        int cpt=0;
        int cpt2=0;
        while (WiFi.status() != WL_CONNECTED) {  //Attente de la connexion
            delay(500);
            //char tmp[10];
            //sprintf(tmp,"%d,",cpt);
            //Serial.print(tmp);
            Serial.print(".");   //Typiquement 5 à 10 points avant la connexion
            if (cpt++ >= 5){
                Serial.println();
                cpt=0;
                WiFi.begin(wifiSsid, wifiPassword);
            }
            if (cpt2++ > 20){
                cpt2=0;
                cpt=0;
                scanNetworks();
            }
        }
        Serial.println("");
        wifiConnected = true;
        Serial.println("WiFi connected");

        // Start the server
        wifiServer.begin();
        Serial.println("Server started");

        // Print the IP address
        Serial.print("Use this URL to connect: ");
        Serial.print("http://");
        IPAddress tmpIp = WiFi.localIP();
        sprintf(ipAdress,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]);
        //Serial.print(WiFi.localIP());
        Serial.print(ipAdress);
        Serial.println("/");  //Utiliser cette URL sous Firefox de preference à Chrome
    } else {
        Serial.println("Aucun reseau wifi disponibles");
        lcd.clear();
        strcpy(wifiSsid,"creer AP");
        strcpy(ipAdress,getDefaultAccesPoint());
        printInfoWifi();
        delay(5000);
    }
}

//=========================================
//
//          getSsid
//
//=========================================
char *getSsid(){
    return wifiSsid;
}

//=========================================
//
//          getIp
//
//=========================================
char *getIp(){
    return ipAdress;
}

//=========================================
//
//          isWifiConnected
//
//=========================================
boolean isWifiConnected(){
    return wifiConnected;
}


