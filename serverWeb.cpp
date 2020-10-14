// ESP32_horloge
// serverWeb.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include "wifiTools.hpp"
#include "reveil.hpp"
#include "heure.hpp"
#include "ntp.hpp"
#include "eeprom.hpp"
#include "temperature.hpp"
#include "config.hpp"

String page;

//=========================================
//
//          displayHeader
//
//=========================================
void displayHeader(void){
    //Serial.println("serverWeb.cpp : displayHeader => debut"); 
    page = "";
    page += ""; //  do not forget this one
    page += "<!DOCTYPE HTML>\n";
    page += "<style type=\"text/css\">\n";
    page += "    div{\n";
    page += "        width:expression(document.body.clientWidth >= 1000? \"1000px\": \"auto\" );\n";
    page += "    }\n";
    page += "    table, th, td {\n";
    page += "        padding: 10px;\n";
    page += "        border: 1px solid black;\n";
    page += "        border-collapse: collapse;\n";
    page += "    }\n";
    page += "</style>\n";
    page += "<html>\n";
    page += "<head><title>Horloge WeMos D1 R1</title>\n";   //Pour l'onglet du navigateur
    page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no\" />\n";
    page += "<meta charset=\"utf-8\" />\n"; 
    //Pour eviter l'emission de  'GET /favicon.ico HTTP/1.1' à chaque requete
    page += "<link rel=\"icon\" href=\"data:,\">\n";
    page += "</head>\n";
    page += "<body>\n";
    page += "<div>\n";
    //Serial.println("serverWeb.cpp : displayHeader => fin"); 
}


//=========================================
//
//          displayFooter
//
//=========================================
void displayFooter(void){
    //Serial.println("serverWeb.cpp : displayFooter => debut"); 
    page += "</div><br><br>\n";
    page += "</body>\n";
    page += "</html>\n";  //Fin de la page
    wifiServer.send( 200, "text/html", page);
    //Serial.println(page);
    //Serial.println("serverWeb.cpp : displayFooter => fin"); 
}

//=========================================
//
//          displayAccesPoints
//
//=========================================
void displayAccesPoints(void){
    char tmpChaine[100];
    //Serial.println("serverWeb.cpp : displayAccesPoints => debut"); 
    displayHeader();
    page += "<h2>Points d'acces Wifi</h2>\n";
    page += "<p>Dans cette page vous pouvez modifier le nom et les mots de pase des points d'accès Wifi auxquels votre horloge est susceptible de se connecter.</p>\n";      //Saut de lignes
    page += "<p>Le premier point d'accès n'est pas modifiable, vous devez l'utiliser lors de l'initialisation pour créer vos points d'accès wifi.</p>\n";      //Saut de lignes
    page += "Pour l'utiliser vous devez créer un point d'accès sur votre mobile avec les parametres (clock/12345678)\n";
    page += "<table>\n";
    page += "    <tr>\n";
    page += "        <th>SSID</th>\n";
    page += "        <th>Passwd</th>\n";
    page += "        <th>Up</th>\n";
    page += "        <th>Down</th>\n";
    page += "        <th>Edit</th>\n";
    page += "    </tr>\n";
    for (int i = 0; i < NB_ACCES_POINTS ; i++){
        page += "    <tr>\n";
        page += "        <td>\n";
        page +=              getSsid(i);
        page += "        </td>\n";
        page += "        <td>\n";
        page +=              getPwd(i);
        page += "        </td>\n";
        if (i > 0){
            page += "        <td>\n";
            if ( i > 1){
                sprintf(tmpChaine,"<a href=\" /updateAccesPoint/up?id=%d\">up</a>\n", i);
            } else {
                sprintf(tmpChaine, "\n");
            }
            page += tmpChaine;
            page += "        </td>\n";
            page += "        <td>\n";
            if ( i < (NB_ACCES_POINTS - 1)){
                sprintf(tmpChaine,"<a href=\" /updateAccesPoint/down?id=%d\">down</a>\n", i);
            } else {
                sprintf(tmpChaine, "\n");
            }
            page += tmpChaine;
            page += "        </td\n>";
            page += "        <td>\n";
            sprintf(tmpChaine,"<a href=\" /updateAccesPoint/edit?id=%d\">edit</a>\n", i);
            page += tmpChaine;
            page += "        </td>\n";
        } else {
        page += "        <td>\n";
        page += "        </td>\n";
        page += "        <td>\n";
        page += "        </td>\n";
        page += "        <td>\n";
        page += "        </td>\n";
        }
        page += "    </tr>\n";
    }
    page += "</table>";
    page += "<br><br>\n";      //Saut de lignes
    page += "<br><a href= \" /accesPoints \" >Refresh</a><br>\n";
    page += "<br><br>\n";      //Saut de lignes
    page += "<br><a href= \" / \" >Retour</a><br>\n";
    page += "<br><br>\n";      //Saut de lignes
    displayFooter();
    //Serial.println("serverWeb.cpp : displayAccesPoints => fin"); 
}

//=========================================
//
//          displayConfigAlarme
//
//=========================================
void displayConfigAlarme(void){
    //Serial.println("serverWeb.cpp : displayConfigAlarme => debut"); 
    displayHeader();
    page += "<h2>Config Alarme</h2>";
    page += "<br><br>";      //Saut de lignes
    page += "<form method=\"get\" action=\"updateAlarme\"  >";
    page += "<table>";
    page += "    <tr>";
    page += "        <th>Paramètre</th>";
    page += "        <th>Valeur</th>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>heure</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"heureReveil\" ";
    page += "                value=";
    page +=                      heureReveil;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>minute</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"minutesReveil\" ";
    page += "                value=";
    page +=                      minutesReveil;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>Actif (on/off)</td>";
    page += "        <td>";
    if (isReveilActif()){
        page += "<a href=\" /switchReveil \">  ON   </a>";
    } else{
        page += "<a href=\" /switchReveil \">  OFF   </a>";
    }
    page += "        </td>";
    page += "    </tr>";
    page += "</table>";
    page += "<input type=\"submit\" value=\"Valider\">";
    page += "</form>";
    page += "<br><br>";      //Saut de lignes
    page += "<br><a href= \" /alarme \" >Refresh</a><br>";
    page += "<br><br>";      //Saut de lignes
    page += "<br><a href= \" / \" >Retour</a><br>";
    page += "<br><br>";      //Saut de lignes
    displayFooter();
    //Serial.println("serverWeb.cpp : displayConfigAlarme => fin"); 
}

//=========================================
//
//          displayTemperature
//
//=========================================
void displayTemperature(void){
    //Serial.println("serverWeb.cpp : displayConfigAlarme => debut"); 
    displayHeader();
    page += "<h2>Temperature / humidité</h2>";
    page += "<br><br>";      //Saut de lignes
    page += "<form method=\"get\" action=\"updateAlarme\"  >";
    page += "<table>";
    page += "    <tr>";
    page += "        <th>Paramètre</th>";
    page += "        <th>Valeur</th>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>Temperature</td>";
    page += "        <td>";
    page +=              getTemperature();
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>Humiditée</td>";
    page += "         <td>";
    page +=              getHumidity();
    page += "        </td>";
    page += "    </tr>";
    page += "</table>";
    page += "</form>";
    page += "<br><br>";      //Saut de lignes
    page += "<br><a href= \" /temperature \" >Refresh</a><br>";
    page += "<br><br>";      //Saut de lignes
    page += "<br><a href= \" / \" >Retour</a><br>";
    page += "<br><br>";      //Saut de lignes
    displayFooter();
    //Serial.println("serverWeb.cpp : displayConfigAlarme => fin"); 
}

//=========================================
//
//          displayConfigHeure
//
//=========================================
void displayConfigHeure(void){
    //Serial.println("serverWeb.cpp : displayConfigHeure => debut"); 
    displayHeader();
    page += "<h2>Config Heure</h2>";
    page += "<br><br>";      //Saut de lignes
    page += "<form method=\"get\" action=\"updateHeure\"  >";
    page += "<table>";
    page += "    <tr>";
    page += "        <th>Parametre</th>";
    page += "        <th>Valeur</th>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>heure</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"heure\" ";
    page += "                value=";
    page +=                      heure;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>minute</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"minutes\" ";
    page += "                value=";
    page +=                      minute;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>secondes</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"second\" ";
    page += "                value=";
    page +=                      second;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>jour</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"jour\" ";
    page += "                value=";
    page +=                      jour;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>mois</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"mois\" ";
    page += "                value=";
    page +=                      mois;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>annee</td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"number\" ";
    page += "                name=\"annee\" ";
    page += "                value=";
    page +=                      annee;
    page += "            />";
    page += "        </td>";
    page += "    </tr>";
    page += "</table>";
    page += "<input type=\"submit\" value=\"Valider\">";
    page += "</form>";
    page += "<br><br>";      //Saut de lignes
    page += "<br><a href= \" /heure \" >Refresh</a><br>";
    page += "<br>";      //Saut de lignes
    page += "<br><a href= \" /updateNTP \" >Mise a jour de l'heure automatique (ntp)</a><br>";
    page += "<br>";      //Saut de lignes
    page += "<br><a href= \" / \" >Retour</a><br>";
    page += "<br><br>";      //Saut de lignes
    displayFooter();
    //Serial.println("serverWeb.cpp : displayConfigHeure => fin"); 
}

//=========================================
//
//          displayHomeMessage
//
//=========================================
void displayHomeMessage(void){
    //Serial.println("serverWeb.cpp : displayHomeMessage => debut"); 
    displayHeader();
    page += "<h2>Horloge avec ESP 32</h2>\n";
    page += "<br>\n";
    page+= "gerer votre horloge sur ESP 32\n";
    page+= "<br><br>\n";
    page+= "Vous allez pouvoir acceder a la configuration des differentes fonctionnalites de votre horloge avec les liens suivants : \n";
    page+= "<br><br>\n";
    page+= "<a href= \" /heure \" >Mise a l'heure</a>\n";
    page+= "<br><br>\n";
    page+= "<a href= \" /alarme \" >Alarme</a>\n";
    page+= "<br><br>\n";
    page+= "<a href= \" /temperature \" >Temperature/humiditée</a>\n";
    page+= "<br><br>\n";
    page+= "<a href= \" /accesPoints \" >Editer les points d'acces Wifi</a>\n";
    page+= "<br><br>\n";
    page+= "<a href= \" /save \" >Sauver les donnees</a>\n";
    page+= "<br><br>\n";
    page+= "<a href= \" /config \" >Configuration des modules à afficher</a>\n";
    page+= "<br><br>\n";
    displayFooter();
    //Serial.println("serverWeb.cpp : displayHomeMessage => fin"); 
}

//=========================================
//
//          displayErrorScreen
//
//=========================================
void displayErrorScreen(void){
    //Serial.println("serverWeb.cpp : displayErrorScreen => debut"); 
    displayHeader();
    page += "<h2>Erreur</h2>\n";
    page += "<br><br>\n";      //Saut de lignes
    page += "Cette page n'existe pas sur ce serveur\n";
    page += "<br><br>\n";      //Saut de lignes
    page += "<br><a href= \" / \" >Retour</a><br>\n";
    page += "<br><br>\n";      //Saut de lignes
    displayFooter();
    //Serial.println("serverWeb.cpp : displayErrorScreen => fin"); 
}

//=========================================
//
//          accesPointUp
//
//=========================================
void accesPointUp()
{
    int index = wifiServer.arg("id").toInt();
    //Serial.printf("accesPointUp %d\n", index);
    if (index > 0){
        //Serial.println("decalage vers le haut possible");
        String tmpSsid = getSsid(index-1);
        String tmpPwd = getPwd(index-1);
        setSsid(index-1,getSsid(index),getPwd(index));
        setSsid(index,tmpSsid,tmpPwd);
    }
    displayAccesPoints();
}

//=========================================
//
//          accesPointdown
//
//=========================================
void accesPointDown()
{
    int index = wifiServer.arg("id").toInt();
    //Serial.printf("accesPointdown %d\n", index);
    if (index < NB_ACCES_POINTS - 1){
        //Serial.println("decalage vers le bas possible");
        String tmpSsid = getSsid(index+1);
        String tmpPwd = getPwd(index+1);
        setSsid(index+1,getSsid(index),getPwd(index));
        setSsid(index,tmpSsid,tmpPwd);
    }
    displayAccesPoints();
}

//=========================================
//
//          accesPointEdit
//
//=========================================
void accesPointEdit()
{
    int index = wifiServer.arg("id").toInt();
    //Serial.println("serverWeb.cpp : accesPointEdit => debut"); 
    displayHeader();
    //Serial.printf("accesPointEdit %d\n", index);
    char tmpChaine[100];
    page += "<h2>Modification d'un point d'acces Wifi</h2>";
    page += "<br><br>";      //Saut de lignes
    page += "<p>Point d'acces numéro : ";
    page += index;
    page += "</p>";
    //sprintf(tmpChaine,"<form method=\"get\" action=\"updateSsid?id=%d \"  >", index);
    sprintf(tmpChaine,"<form method=\"get\" action=\"updateSsid\"  >");
    //Serial.println(tmpChaine);
    page += tmpChaine;
    page += "<input";
    page += "   type=\"string\" ";
    page += "   name=\"index\" ";
    page += "   value=";
    sprintf(tmpChaine,"%d",index);
    page +=     tmpChaine;
    page += ">";
    page += "<table>";
    page += "    <tr>";
    page += "        <th>SSID</th>";
    page += "        <th>Passwd</th>";
    page += "    </tr>";
    page += "    <tr>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"string\" ";
    page += "                name=\"ssid\" ";
    page += "                value=";
    page +=                      getSsid(index);
    page += "            >";
    page += "        </td>";
    page += "        <td>";
    page += "            <input";
    page += "                type=\"string\" ";
    page += "                name=\"pwd\" ";
    page += "                value=";
    page +=                      getPwd(index);
    page += "            >";
    page += "        </td>";
    page += "    </tr>";
    page += "</table>";
    page += "<br><br>";      //Saut de lignes
    page += "<input type=\"submit\" value=\"Valider\">";
    page += "</form>";
    page += "<br><br>";      //Saut de lignes
    page += "<br><a href= \" / \" >Retour</a><br>";
    page += "<br><br>";      //Saut de lignes
    displayFooter();
    //Serial.println("serverWeb.cpp : accesPointEdit => fin"); 
}

//=========================================
//
//          updateSsid
//
//=========================================
void updateSsid(){
    //Serial.printf("serverWeb.cpp : updateSsid => debut\n");
    String ssid = wifiServer.arg("ssid");
    String pwd = wifiServer.arg("pwd");
    String idx = wifiServer.arg("index");
    //Serial.println("    Parametres d'entrée :");
    //Serial.print("       index = ");
    //Serial.println(idx);
    //Serial.print("       ssid = ");
    //Serial.println(ssid);
    //Serial.print("       pwd = ");
    //Serial.println(pwd);
    setSsid(idx.toInt(), ssid, pwd);
    displayAccesPoints();
    //Serial.printf("serverWeb.cpp : updateSsid => debut\n");
}

//=========================================
//
//          updateNtp
//
//=========================================
void updateNtp(){
    //Serial.println("serverWeb.cpp : updateNtp => debut");
    refreshNtpNow();
    displayConfigHeure();
    //Serial.println("serverWeb.cpp : updateNtp => fin");
}

//=========================================
//
//          setHeure
//
//=========================================
void setHeure(){
    //Serial.println("serverWeb.cpp : setHeure => debut");
    // TODO check validitee des données recues
    int tmp;
    tmp = wifiServer.arg("heure").toInt();
    if (tmp >= 0 && tmp <= 23){
        heure = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour heure incorrecte");
    }
    tmp = wifiServer.arg("minutes").toInt();
    if (tmp >= 0 && tmp <= 60){
        minute = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour minute incorrecte");
    }
    tmp = wifiServer.arg("second").toInt();
    if (tmp >= 0 && tmp <= 60){
        second = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour second incorrecte");
    }
    tmp = wifiServer.arg("jour").toInt();
    if (tmp >= 0 && tmp <= 31){
        jour = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour jour incorrecte");
    }
    tmp = wifiServer.arg("mois").toInt();
    if (tmp >= 0 && tmp <= 31){
        mois = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour mois incorrecte");
    }
    tmp = wifiServer.arg("annee").toInt();
    if (tmp >= 1970){
        annee = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour annee incorrecte");
    }
    displayConfigHeure();
    //Serial.println("serverWeb.cpp : setHeure => fin");
}

//=========================================
//
//          setAlarme
//
//=========================================
void setAlarme(){
    //Serial.println("serverWeb.cpp : setAlarme => debut");
    int tmp = wifiServer.arg("heureReveil").toInt();
    if (tmp >= 0 && tmp <= 23){
        heureReveil = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour heureReveil incorrecte");
    }
    tmp = wifiServer.arg("minutesReveil").toInt();
    if (tmp >= 0 && tmp <= 60){
        minutesReveil = tmp;
    } else {
        Serial.print(" valeur ");
        Serial.print(tmp);
        Serial.println(" pour minutesReveil incorrecte");
    }
    displayConfigAlarme();
    //Serial.println("serverWeb.cpp : setAlarme => fin");
}

//=========================================
//
//          switchReveil
//
//=========================================
void switchReveil(){
    //Serial.println("serverWeb.cpp : switchReveil => debut");
    switchReveilOnOff();
    displayConfigAlarme();
    //Serial.println("serverWeb.cpp : switchReveil => fin");
}

//=========================================
//
//          save
//
//=========================================
void save(){
    //Serial.println("serverWeb.cpp : save => debut");
    saveDatasToFlash();
    displayHomeMessage();
    //Serial.println("serverWeb.cpp : save => fin");
}

//=========================================
//
//          configWeb
//
//=========================================
void configWeb(){
    //Serial.println("serverWeb.cpp : configWeb => debut");
    char tmp[100];
    displayHeader();
    page += "<h2>Configuration</h2>\n";
    page += "<br><br>\n";      //Saut de lignes
    page += "Cette page permet de configurer les services disponibles sur ce module\n";
    page += "<table>";
    page += "    <tr>";
    page += "        <th>Service</th>";
    page += "        <th>Activation</th>";
    page += "    </tr>";
    for (int i = 0 ; i < NBDISPLAYMODE ; i++){
        page += "    <tr>";
        page += "        <td>";
        page +=             tblModeDisplay[i];
        page += "        </td>";
        page += "        <td>";
        sprintf(tmp, "<a href=\" /switchModuleConfig?id=%d \">%d</a>", i, tblConfigModule[i]);
        page +=            tmp;
        page += "        </td>";
        page += "    </tr>";
    }
    page += "</table>";
    page += "<br><br>\n";      //Saut de lignes
    page += "<br><a href= \" / \" >Retour</a><br>\n";
    page += "<br><br>\n";      //Saut de lignes
    displayFooter();
    //Serial.println("serverWeb.cpp : configWeb => fin");
}

//=========================================
//
//          switchConfig
//
//=========================================
void switchConfig(){
    switchModuleStatus(wifiServer.arg("id").toInt());
    configWeb();
}

//=========================================
//
//          serverWebInit
//
//=========================================
void serverWebInit(){
    // initialisation des points d'entrée du server serverWebInit
    // home page
    wifiServer.on("/", displayHomeMessage);
    // gestion de l'heure
    wifiServer.on("/heure", displayConfigHeure);
    wifiServer.on("/updateNTP", updateNtp);
    wifiServer.on("/updateHeure", setHeure);
    // gestion de l'alarme
    wifiServer.on("/alarme", displayConfigAlarme);
    wifiServer.on("/updateAlarme", setAlarme);
    wifiServer.on("/switchReveil", switchReveil);
    // gestion de la sauvegarde
    wifiServer.on("/save", save);
    // gestion des points d'acces wifi
    wifiServer.on("/accesPoints", displayAccesPoints);
    wifiServer.on("/updateAccesPoint/edit", accesPointEdit);
    wifiServer.on("/updateAccesPoint/down", accesPointDown);
    wifiServer.on("/updateAccesPoint/up", accesPointUp);
    wifiServer.on("/updateAccesPoint/updateSsid", updateSsid);
    // affichage de la temperature
    wifiServer.on("/temperature",displayTemperature);
    // page de configuration
    wifiServer.on("/config",configWeb);
    wifiServer.on("/switchModuleConfig",switchConfig);
    // page inexistante
    wifiServer.onNotFound(displayErrorScreen); 
    //Serial.println("serverWeb.cpp : serverWebInit => fin");
}
