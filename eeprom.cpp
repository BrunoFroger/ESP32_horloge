// ESP32_horloge
// eeprom.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <EEPROM.h>
#include "reveil.hpp"
#include "eeprom.hpp"
#include <String.h>


#define EEPROM_SIZE     768
#define EEPROM_START    0
#define SSID_SIZE       25
#define PWD_SIZE        30

typedef struct {
    char ssid[SSID_SIZE];
    char pwd[PWD_SIZE];
} accesPoint;

struct{
    char eepromInitialized[15];
    int heureReveil;
    int minutesReveil;
    boolean reveilActif;
    accesPoint tblAccesPoint[NB_ACCES_POINTS];
} storedDatas;

boolean storageAvailable;
boolean datasToUpdate;

char defaultAccesPointSsid[10] = "clock";
char defaultAccesPointPwd[10] = "12345678";
char defaultAccesPoint[50];

//=========================================
//
//          booleanToString
//
//=========================================
char *booleanToString(boolean value){
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
    
    Serial.print  ("| Initialisation     |");
    //tmp = String(storedDatas.heureReveil);
    //tmp1 = String(heureReveil);
    Serial.printf (" %14s  | %14s  |\n", storedDatas.eepromInitialized, "NC");
    //Serial.printf (" %14s  | %14s  |\n", tmp, tmp1);

    Serial.print  ("| heure de reveil    |");
    Serial.printf (" %14d  | %14d  |\n", storedDatas.heureReveil, heureReveil);

    Serial.print  ("| minutes de reveil  |");
    //tmp = String(storedDatas.minutesReveil);
    //tmp1 = String(minutesReveil);
    Serial.printf (" %14d  | %14d  |\n", storedDatas.minutesReveil, minutesReveil);

    Serial.print  ("| reveil on/off      |");
    //tmp = booleanToString(storedDatas.reveilActif);
    //tmp1 = booleanToString(isReveilActif());
    Serial.printf (" %14s  | %14s  |\n", booleanToString(storedDatas.reveilActif), booleanToString(isReveilActif()));
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
//          getDefaultAccesPoint
//
//=========================================
char *getDefaultAccesPoint(){
    return defaultAccesPoint;
}

//=========================================
//
//          initTblAccesPoints
//
//=========================================
void initTblAccesPoints(){
    Serial.println("initTblAccesPoints => debut");
    int dataSize = sizeof(storedDatas);
    Serial.print("Taille de la structure de donnees a sauvegarder : ");
    Serial.println(dataSize);
    Serial.print("Taille memoire reservee en eeprom : ");
    Serial.println(EEPROM_SIZE);
    if (dataSize > EEPROM_SIZE){
        Serial.println("ERREUR : taille demandee trop grande pour la place reservee");
    }
    Serial.println("Initialisation du tableau des points d'acces wifi");
    sprintf(defaultAccesPoint,"%s/%s", defaultAccesPointSsid, defaultAccesPointPwd);
    setSsid(0, defaultAccesPointSsid, defaultAccesPointPwd);
    for (int i = 1; i < NB_ACCES_POINTS ; i++){
        setSsid(i, "", "");
    }
    Serial.println("initTblAccesPoints => fin");
}

//=========================================
//
//          storageError
//
//=========================================
void storageError(int delta){
    Serial.print("ERROR : taille structure de donnéées a sauvegarder (");
    Serial.print(sizeof(storedDatas));
    Serial.println(") ");
    Serial.print("est supérieure a la taille reservée (");
    Serial.print(EEPROM_SIZE);
    Serial.print(")");
    Serial.print("difference de taille (");
    Serial.print(delta);
    Serial.print(")");
    Serial.println();
    Serial.flush();
}

//=========================================
//
//          restoreDatasfromFlash
//
//=========================================
void restoreDatasfromFlash(boolean storageAvailable){
    Serial.println("restoreDatasfromFlash => debut");
    if (!storageAvailable){
        Serial.println("EEPROM not ready to save datas");
        Serial.println("restoreDatasfromFlash => fin");
        return;
    }
    EEPROM.get(EEPROM_START, storedDatas);
    Serial.println("On vient de lire les donneees en EEprom");
    displayStoredDatasStructure();
    if (strcmp(storedDatas.tblAccesPoint[0].ssid,"clock") != 0) {
        Serial.println("ERROR : Le PA clock n'est pas en eeprom, on invalide eepromInitialized");
        strcpy(storedDatas.eepromInitialized,"");
    }
    if (strcmp(storedDatas.eepromInitialized,"initialized")!= 0){
        Serial.println("ERROR : EEPROM not initialized");
        initSaveToFlash();
        Serial.println("restoreDatasfromFlash => fin");
        return;
    } 
    heureReveil = storedDatas.heureReveil;
    minutesReveil = storedDatas.minutesReveil;
    if (storedDatas.reveilActif != isReveilActif()){
        switchReveilOnOff();
    }
    displayStoredDatasStructure();
    Serial.println("restoreDatasfromFlash => fin");
}

//=========================================
//
//          initSaveToFlash
//
//=========================================
void initSaveToFlash(void){
    Serial.println("initSaveToFlash => debut");
    initTblAccesPoints();
    if (sizeof(storedDatas) > EEPROM_SIZE){
        storageAvailable = false;
        storageError(sizeof(storedDatas) - EEPROM_SIZE);
    } else {
        Serial.println("Init Eeprom OK");
        EEPROM.begin(EEPROM_SIZE);
        storageAvailable = true;
        restoreDatasfromFlash(storageAvailable);
        Serial.println("datas recuperees de l'eeprom");
        // TODO sauvegarde provisoire de initTblAccesPoints vide
        //saveDatasToFlash();
        Serial.print("Valeur de eeprom Initialized : ");
        Serial.println(storedDatas.eepromInitialized);
        if (strcmp(storedDatas.eepromInitialized,"initialized") != 0){
            Serial.println("initSaveToFlash => Initialisation de l'eeprom avec valeur origine");
            storedDatas.heureReveil=-1; // to force update of eeprom
            storedDatas.minutesReveil=-1;
            storedDatas.reveilActif=false;
            Serial.println("datas initialisée dans l'eeprom");
            saveDatasToFlash();
        } 
        datasToUpdate=false;
    }
    Serial.println("initSaveToFlash => fin");
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
    Serial.println("saveDatasToFlash => debut");
    if (storageAvailable){
        if (savetoFlashNeeded()){
            strcpy(storedDatas.eepromInitialized,"initialized");
            storedDatas.heureReveil = heureReveil;
            storedDatas.minutesReveil = minutesReveil;
            storedDatas.reveilActif = isReveilActif();
            EEPROM.put(0, storedDatas);
            EEPROM.commit();
            Serial.println("Sauvegarde des données en Eeprom OK");
            datasToUpdate = false;
        } else {
            Serial.println("Sauvegarde des données en Eeprom inutile, pas de changement dans les valeurs");
        }
    } else {
        Serial.println("EEPROM not ready to save datas");
    }
    Serial.println("saveDatasToFlash => fin");
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
    /*Serial.printf("Mise a jour du ssid %d avec les valeurs\n", index);
    Serial.print("ssid : ");
    Serial.println(ssid);
    Serial.print("pwd  : ");
    Serial.println(passwd);*/
    ssid.toCharArray(storedDatas.tblAccesPoint[index].ssid, ssid.length()+1);
    passwd.toCharArray(storedDatas.tblAccesPoint[index].pwd, passwd.length()+1);
    datasToUpdate = true;
}



//=========================================
//
//          isAvailableAccesPoint
//
//=========================================
int isAvailableAccesPoint(String ssid){
    char tmp[SSID_SIZE];
    ssid.toCharArray(tmp,SSID_SIZE);
    for (int i = 0 ; i < NB_ACCES_POINTS ; i++){
        //Serial.print("      compare avec le PA enregistre : ");
        //Serial.print(storedDatas.tblAccesPoint[i].ssid);
        if (strcmp(storedDatas.tblAccesPoint[i].ssid,tmp) == 0){
            //Serial.println(" => OK");
            return i;
        }
        //Serial.println(" => NOK");
    }
    return -1;
}