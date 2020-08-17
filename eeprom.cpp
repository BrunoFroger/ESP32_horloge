// ESP32_horloge
// eeprom.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <EEPROM.h>
#include "reveil.hpp"
#include "eeprom.hpp"


#define EEPROM_SIZE     768
#define EEPROM_START    0
#define SSID_SIZE       25
#define PWD_SIZE        30

typedef struct {
    char ssid[SSID_SIZE];
    char pwd[PWD_SIZE];
} accesPoint;

struct{
    int heureReveil;
    int minutesReveil;
    boolean reveilActif;
    accesPoint tblAccesPoint[NB_ACCES_POINTS];
} storedDatas;

boolean storageAvailable;
boolean datasToUpdate;


//=========================================
//
//          booleanToString
//
//=========================================
String booleanToString(boolean value){
    if (value){
        return "true";
    }
    return "false";
}

//=========================================
//
//          displayStoredDatasStructure
//
//=========================================
void displayStoredDatasStructure(void){
    String tmp, tmp1;
    Serial.println("+--------------------------------------------------------+");
    Serial.println("|           Structure de données a sauvegarder           |");
    Serial.println("+--------------------+-----------------------------------+");
    Serial.println("| nom                | valeur eeprom   | valeur memoire  |");
    Serial.println("+--------------------+-----------------+-----------------+");
    Serial.print  ("| heure de reveil    |");
    tmp = String(storedDatas.heureReveil);
    tmp1 = String(heureReveil);
    Serial.printf (" %14s  | %14s  |\n", tmp, tmp1);

    Serial.print  ("| minutes de reveil  |");
    tmp = String(storedDatas.minutesReveil);
    tmp1 = String(minutesReveil);
    Serial.printf (" %14s  | %14s  |\n", tmp, tmp1);

    Serial.print  ("| reveil on/off      |");
    tmp = booleanToString(storedDatas.reveilActif);
    tmp1 = booleanToString(isReveilActif());
    Serial.printf (" %14s  | %14s  |\n", tmp, tmp1);
    Serial.println("+--------------------+-----------------+-----------------+");

    Serial.println();
    Serial.println("+----------------------------------------------------------+");
    Serial.println("|                  Points d'acces Wifi                     |");
    Serial.println("+----------------------------------------------------------+");
    Serial.println("| ssid                     | passwd                        |");
    Serial.println("+--------------------------+-------------------------------+");
    char tmpchaine[80];
    for (int i = 0 ; i < NB_ACCES_POINTS ; i++){
        sprintf(tmpchaine, "|%25s |%30s |", storedDatas.tblAccesPoint[i].ssid, storedDatas.tblAccesPoint[i].pwd);
        Serial.println(tmpchaine);
    }
    Serial.println("+--------------------------+-------------------------------+");
}


//=========================================
//
//          initTblAccesPoints
//
//=========================================
void initTblAccesPoints(){
    int dataSize = sizeof(storedDatas);
    Serial.print("Taille de la structure de donnees a sauvegarder : ");
    Serial.println(dataSize);
    Serial.print("Taille memoire reservee en eeprom : ");
    Serial.println(EEPROM_SIZE);
    if (dataSize > EEPROM_SIZE){
        Serial.println("ERREUR : taille demandee trop grande pour la place reservee");
    }
    Serial.println("Initialisation du tableau des points d'acces wifi");
    setSsid(0, "clock", "1234");
    for (int i = 1; i < NB_ACCES_POINTS ; i++){
        setSsid(i, "", "");
        
    }
}

//=========================================
//
//          storageError
//
//=========================================
void storageError(void){
    Serial.print("ERROR : taille structure de donnéées a sauvegarder (");
    Serial.print(sizeof(storedDatas));
    Serial.println(") ");
    Serial.print("est supérieure a la taille reservée (");
    Serial.print(EEPROM_SIZE);
    Serial.print(")");
    Serial.println();
    Serial.flush();
}

//=========================================
//
//          restoreDatasfromFlash
//
//=========================================
void restoreDatasfromFlash(void){
    if (storageAvailable){
        EEPROM.get(EEPROM_START, storedDatas);
        heureReveil = storedDatas.heureReveil;
        minutesReveil = storedDatas.minutesReveil;
        if (storedDatas.reveilActif != isReveilActif()){
            switchReveilOnOff();
        }
        // TODO recuperation des points d'accs wifi
        displayStoredDatasStructure();
    } else {
        storageError();
    }
}

//=========================================
//
//          initSaveToFlash
//
//=========================================
void initSaveToFlash(void){
    Serial.println("Debut de initSaveToFlash");
    initTblAccesPoints();
    if (sizeof(storedDatas) > EEPROM_SIZE){
        storageAvailable = false;
        storageError();
    } else {
        Serial.println("Init Eeprom OK");
        EEPROM.begin(EEPROM_SIZE);
        storageAvailable = true;
        // TODO sauvegarde provisoire de initTblAccesPoints vide
        //saveDatasToFlash();
        restoreDatasfromFlash();
        Serial.println("datas recuperees de l'eeprom");
        datasToUpdate=false;
    }
    Serial.println("Fin de initSaveToFlash");
}

//=========================================
//
//          savetoFlashNeeded
//
//=========================================
boolean savetoFlashNeeded(void){
    if (storedDatas.heureReveil != heureReveil){
        //Serial.println("heureReveil a changé");
        datasToUpdate = true;
    }
    if (storedDatas.minutesReveil != minutesReveil){
        //Serial.println("minutesReveil a changé");
        datasToUpdate = true;
    }
    if (storedDatas.reveilActif != isReveilActif()){
        //Serial.println("l'activation du reveil a changé");
        datasToUpdate = true;
    }
    //Serial.print("savetoFlashNeeded : ");
    //Serial.println(datasToUpdate);
    displayStoredDatasStructure();
    return (datasToUpdate);
}

//=========================================
//
//          saveDatasToFlash
//
//=========================================
void saveDatasToFlash(void){
    if (storageAvailable){
        if (savetoFlashNeeded()){
            storedDatas.heureReveil = heureReveil;
            storedDatas.minutesReveil = minutesReveil;
            storedDatas.reveilActif = isReveilActif();
            EEPROM.put(0, storedDatas);
            EEPROM.commit();
            Serial.println("Sauvegarde des données en Eeprom OK");
            datasToUpdate = false;
        //} else {
            //Serial.println("Sauvegarde des données en Eeprom inutile, pas de changement dans les valeurs");
        }
    } else {
        storageError();
    }
}


//=========================================
//
//          getSsid
//
//=========================================
String getSsid(int index){
    return storedDatas.tblAccesPoint[index].ssid;
}

//=========================================
//
//          getPwd
//
//=========================================
String getPwd(int index){
    return storedDatas.tblAccesPoint[index].pwd;
}

//=========================================
//
//          setSsid
//
//=========================================
void setSsid(int index, String ssid, String passwd){
    Serial.printf("Mise a jour du ssid %d avec les valeurs\n", index);
    Serial.print("ssid : ");
    Serial.println(ssid);
    Serial.print("pwd  : ");
    Serial.println(passwd);
    ssid.toCharArray(storedDatas.tblAccesPoint[index].ssid, ssid.length()+1);
    passwd.toCharArray(storedDatas.tblAccesPoint[index].pwd, passwd.length()+1);
    datasToUpdate = true;
}
