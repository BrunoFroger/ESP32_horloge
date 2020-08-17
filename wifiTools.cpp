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
                    strcpy(wifiSsid,tmp);
                    getPwd(availableSsidIndex).toCharArray(tmp,30);
                    strcpy(wifiPassword,tmp);
                    Serial.println(" => OK");
                    break;
                }
                Serial.println(" => NOK");
                /*
                if (WiFi.SSID(ssidNetwork) == "NETGEAR17"){
                    strcpy(wifiSsid,"NETGEAR17");
                    strcpy(wifiPassword, "largesea818");
                    Serial.println(" => OK");
                    break;
                }
                if (WiFi.SSID(ssidNetwork) == "Livebox-006d"){
                    strcpy(wifiSsid,"Livebox-006d");
                    strcpy(wifiPassword, "A23F1F7979C9DD3C5916324123");
                    Serial.println(" => OK");
                    break;
                }
                if (WiFi.SSID(ssidNetwork) == "TP-LINK_88E2"){
                    strcpy(wifiSsid,"TP-LINK_88E2");
                    strcpy(wifiPassword, "75546007");
                    Serial.println(" => OK");
                    break;
                }
                if (WiFi.SSID(ssidNetwork) == "AndroidAPBruno"){
                    strcpy(wifiSsid,"AndroidAPBruno");
                    strcpy(wifiPassword, "0296072588");
                    Serial.println(" => OK");
                    break;
                }
                Serial.println(" => NOK");
                */
            }
            if (strcmp(wifiSsid,"") == 0){
                delay(2000);
                Serial.println("No Wifi network found ==> rescan ......");
                nbSsid = WiFi.scanNetworks();
            }
        }
    }
}



//=========================================
//
//          initWifi
//
//=========================================
void initWifi(void){    // init wifi connection

    deconnecteWifi();
    delay(1000);
    scanNetworks();
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



