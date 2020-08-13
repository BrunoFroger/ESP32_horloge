// ESP32_horloge
// eeprom.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <EEPROM.h>
#include "reveil.hpp"


#define EEPROM_SIZE     64
#define EEPROM_START    0

struct{
    int heureReveil;
    int minutesReveil;
    boolean reveilActif;
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
        Serial.println("Restauration des données sauvegardées en Eeprom OK");
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
    if (sizeof(storedDatas) > EEPROM_SIZE){
        storageAvailable = false;
        storageError();
    } else {
        Serial.println("Init Eeprom OK");
        EEPROM.begin(EEPROM_SIZE);
        storageAvailable = true;
        restoreDatasfromFlash();
        datasToUpdate=false;
    }
}

//=========================================
//
//          savetoFlashNeeded
//
//=========================================
boolean savetoFlashNeeded(void){
    datasToUpdate = false;
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
        //} else {
            //Serial.println("Sauvegarde des données en Eeprom inutile, pas de changement dans les valeurs");
        }
    } else {
        storageError();
    }
}

