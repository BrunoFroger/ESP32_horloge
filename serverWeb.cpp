// ESP32_horloge
// serverWeb.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include "wifiTools.hpp"
#include "reveil.hpp"
#include "heure.hpp"
#include "ntp.hpp"
#include "eeprom.hpp"

//=========================================
//
//          displayAccesPoints
//
//=========================================
void displayAccesPoints(void){
    char tmpChaine[100];
    //Serial.println("displayAccesPoints"); 
    wifiClient.println("<h2>Points d'acces Wifi</h2>");
    wifiClient.println("<p>Dans cette page vous pouvez modifier le nom et les mots de pase des points d'accès Wifi auxquels votre horloge est susceptible de se connecter.</p>");      //Saut de lignes
    wifiClient.println("<p>Le premier point d'accès n'est pas modifiable, vous devez l'utiliser lors de l'initialisation pour créer vos points d'accès wifi.</p>");      //Saut de lignes
    wifiClient.println("Pour l'utiliser vous devez créer un point d'accès sur votre mobile avec les parametres (clock/1234)");
    wifiClient.println("<table>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <th>SSID</th>");
    wifiClient.println("        <th>Passwd</th>");
    wifiClient.println("        <th>Up</th>");
    wifiClient.println("        <th>Down</th>");
    wifiClient.println("        <th>Edit</th>");
    wifiClient.println("    </tr>");
    for (int i = 0; i < NB_ACCES_POINTS ; i++){
        wifiClient.println("    <tr>");
        wifiClient.println("        <td>");
        wifiClient.println(             getSsid(i));
        wifiClient.println("        </td>");
        wifiClient.println("        <td>");
        wifiClient.println(             getPwd(i));
        wifiClient.println("        </td>");
        if (i > 0){
            wifiClient.println("        <td>");
            if ( i > 1){
                sprintf(tmpChaine,"<a href=\" /updateAccesPoint/up?id=%d\">up</a>", i);
            } else {
                sprintf(tmpChaine, "");
            }
            //Serial.println(tmpChaine);
            wifiClient.println(tmpChaine);
            wifiClient.println("        </td>");
            wifiClient.println("        <td>");
            if ( i < (NB_ACCES_POINTS - 1)){
                sprintf(tmpChaine,"<a href=\" /updateAccesPoint/down?id=%d\">down</a>", i);
            } else {
                sprintf(tmpChaine, "");
            }
            //Serial.println(tmpChaine);
            wifiClient.println(tmpChaine);
            wifiClient.println("        </td>");
            wifiClient.println("        <td>");
            sprintf(tmpChaine,"<a href=\" /updateAccesPoint/edit?id=%d\">edit</a>", i);
            wifiClient.println(tmpChaine);
            wifiClient.println("        </td>");
        } else {
            wifiClient.println("        <td>");
            wifiClient.println("        </td>");
            wifiClient.println("        <td>");
            wifiClient.println("        </td>");
            wifiClient.println("        <td>");
            wifiClient.println("        </td>");
        }
        wifiClient.println("    </tr>");
    }
    wifiClient.println("</table>");
    wifiClient.println("<input type=\"submit\" value=\"Valider\">");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" /accesPoints \" >Refresh</a><br>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" / \" >Retour</a><br>");
    wifiClient.println("<br><br>");      //Saut de lignes
}

//=========================================
//
//          displayConfigAlarme
//
//=========================================
void displayConfigAlarme(void){
    //Serial.println("displayConfigAlarme"); 
    wifiClient.println("<h2>Config Alarme</h2>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<form method=\"get\" action=\"updateAlarme\"  >");
    wifiClient.println("<table>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <th>Paramètre</th>");
    wifiClient.println("        <th>Valeur</th>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>heure</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"heureReveil\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     heureReveil);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>minute</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"minutesReveil\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     minutesReveil);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>Actif (on/off)</td>");
    wifiClient.println("        <td>");
    if (isReveilActif()){
        wifiClient.println("<a href=\" /switchReveil \">  ON   </a>");
    } else{
        wifiClient.println("<a href=\" /switchReveil \">  OFF   </a>");
    }
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("</table>");
    wifiClient.println("<input type=\"submit\" value=\"Valider\">");
    wifiClient.println("</form>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" /alarme \" >Refresh</a><br>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" / \" >Retour</a><br>");
    wifiClient.println("<br><br>");      //Saut de lignes
}

//=========================================
//
//          displayConfigHeure
//
//=========================================
void displayConfigHeure(void){
    //Serial.println("displayConfigHeure"); 
    wifiClient.println("<h2>Config Heure</h2>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<form method=\"get\" action=\"updateHeure\"  >");
    wifiClient.println("<table>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <th>Paramètre</th>");
    wifiClient.println("        <th>Valeur</th>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>heure</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"heure\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     heure);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>minute</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"minutes\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     minute);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>secondes</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"second\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     second);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>jour</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"jour\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     jour);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>mois</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"mois\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     mois);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>annee</td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"number\" ");
    wifiClient.println("                name=\"annee\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     annee);
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("</table>");
    wifiClient.println("<input type=\"submit\" value=\"Valider\">");
    wifiClient.println("</form>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" /heure \" >Refresh</a><br>");
    wifiClient.println("<br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" /updateNTP \" >Mise a jour de l'heure automatique (ntp)</a><br>");
    wifiClient.println("<br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" / \" >Retour</a><br>");
    wifiClient.println("<br><br>");      //Saut de lignes
}

//=========================================
//
//          displayHeader
//
//=========================================
void displayHeader(void){
    //Serial.println("displayHeader"); 
    wifiClient.println("HTTP/1.1 200 OK");
    wifiClient.println("Content-Type: text/html");
    wifiClient.println(""); //  do not forget this one
    wifiClient.println("<!DOCTYPE HTML>");
    wifiClient.println("<style type=\"text/css\">");
    wifiClient.println("    div {");
    wifiClient.println("        width:expression(document.body.clientWidth >= 1000? \"1000px\": \"auto\" );");
    wifiClient.println("    }");
    wifiClient.println("    table, th, td {");
    wifiClient.println("        padding: 10px;");
    wifiClient.println("        border: 1px solid black;");
    wifiClient.println("        border-collapse: collapse;");
    wifiClient.println("    }");
    wifiClient.println("</style>");
    wifiClient.println("<html>");
    wifiClient.println("<head><title>Horloge WeMos D1 R1</title>");   //Pour l'onglet du navigateur
    wifiClient.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no\" />"); 
    //wifiClient.println("<meta http-equiv=\"refresh\" content=\"5\" />");
    wifiClient.println("<meta charset=\"utf-8\" />"); 

    //Pour eviter l'emission de  'GET /favicon.ico HTTP/1.1' à chaque requete
    wifiClient.println("<link rel=\"icon\" href=\"data:,\">");

    wifiClient.println("</head>");
    wifiClient.println("<body>");
    //wifiClient.println("=============================================================================================");
    wifiClient.println("<div>");
}


//=========================================
//
//          displayFooter
//
//=========================================
void displayFooter(void){
    //Serial.println("displayFooter"); 
    wifiClient.println("</div><br><br>");
    //wifiClient.println("=============================================================================================");
    wifiClient.println("</body>");
    wifiClient.println("</html>");  //Fin de la page
    //Serial.println("Client disconnected");  //Aller attendre une nouvelle demande du Client
    //Serial.println("");
}


//=========================================
//
//          displayHomeMessage
//
//=========================================
void displayHomeMessage(void){
    //Serial.println("displayHomeMessage"); 
    wifiClient.println("<h2>Horloge avec ESP 32</h2>");
    wifiClient.println("<br>");      //Saut de lignes
    wifiClient.println("gerer votre horloge sur ESP 32");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("Vous allez pouvoir acceder a la configuration des differentes fonctionnalites de votre horloge avec les liens suivants : ");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<a href= \" /heure \" >Mise a l'heure</a>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<a href= \" /alarme \" >Alarme</a>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<a href= \" /accesPoints \" >Editer les points d'acces Wifi</a>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<a href= \" /save \" >Sauver les donnees</a>");
    wifiClient.println("<br><br>");      //Saut de lignes
    //wifiClient.println("<a href= \" /dfgsdgfqdfsg \" >test page erreur</a>");
    //wifiClient.println("<br><br>");      //Saut de lignes
}

//=========================================
//
//          displayErrorScreen
//
//=========================================
void displayErrorScreen(void){
    wifiClient.println("<h2>Erreur</h2>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("Cette page n'existe pas sur ce serveur");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" / \" >Retour</a><br>");
    wifiClient.println("<br><br>");      //Saut de lignes
}

//=========================================
//
//          updateVariable
//
//=========================================
void updateVariable(String requete){
    //Serial.println("updateVariable"); 
    //Serial.print("requete recue : ");
    //Serial.print(requete);
    //Serial.println();
    int index = requete.indexOf('=');
    String name = requete.substring(0,index);
    String value = requete.substring(index+1,requete.length());
    
    int tmp = value.toInt();
    /*
    Serial.print("nom de la variable : <");
    Serial.print(name);
    Serial.print("> ; valeur : <");
    Serial.print(tmp);
    Serial.println(">");*/
    
    if (name.equals("heureReveil")){
        if ((tmp >= 0) && (tmp < 24)){
            heureReveil = tmp;
        }
    } else if (name.equals("minutesReveil")){
        if (tmp >= 0 && tmp < 60){
            minutesReveil = tmp;
        }
    } else if (name.equals("heure")){
        if (tmp >= 0 && tmp < 24){
            heure = tmp;
        }
    } else if (name.equals("minute")){
        if (tmp >= 0 && tmp < 24){
            minute = tmp;
        }
    } else if (name.equals("second")){
        if (tmp >= 0 && tmp < 60){
            second = tmp;
        }
    } else if (name.equals("jour")){
        if (tmp >= 0 && tmp < 32){
            jour = tmp;
        }
    } else if (name.equals("mois")){
        if (tmp >= 0 && tmp < 13){
            mois = tmp;
        }
    } else if (name.equals("annee")){
        annee = tmp;
    } else {
        Serial.print("ERREUR : nom de variable inconnu : ");
        Serial.print(name);
        Serial.println();
    }
}

//=========================================
//
//          update
//
//=========================================
void update(String request){
    String tmp;
    /*Serial.println("update"); 
    Serial.print("requete recue : ");
    Serial.print(request);
    Serial.println();*/
    int index1 = request.lastIndexOf("?");
    int index2 = request.lastIndexOf(" HTTP");
    tmp=request.substring(index1+1,index2);
    while (tmp.indexOf('&') != -1){
        String item = tmp.substring(0,tmp.indexOf('&'));
        //Serial.print("mise a jour de ");
        //Serial.println(item);
        updateVariable(tmp.substring(0,tmp.indexOf('&')));
        tmp=tmp.substring(tmp.indexOf('&')+1, tmp.length());
    }
    updateVariable(tmp);
}

//=========================================
//
//          accesPointUp
//
//=========================================
void accesPointUp(int index)
{
    Serial.printf("accesPointUp %d\n", index);
    if (index > 0){
        Serial.println("decalage vers le haut possible");
        String tmpSsid = getSsid(index-1);
        String tmpPwd = getPwd(index-1);
        setSsid(index-1,getSsid(index),getPwd(index));
        setSsid(index,tmpSsid,tmpPwd);
    }
}

//=========================================
//
//          accesPointdown
//
//=========================================
void accesPointdown(int index)
{
    Serial.printf("accesPointdown %d\n", index);
    if (index < NB_ACCES_POINTS - 1){
        Serial.println("decalage vers le bas possible");
        String tmpSsid = getSsid(index+1);
        String tmpPwd = getPwd(index+1);
        setSsid(index+1,getSsid(index),getPwd(index));
        setSsid(index,tmpSsid,tmpPwd);
    }
}

//=========================================
//
//          accesPointEdit
//
//=========================================
void accesPointEdit(int index)
{
    Serial.printf("accesPointEdit %d\n", index);
    char tmpChaine[100];
    wifiClient.println("<h2>Modification d'un point d'acces Wifi</h2>");
    wifiClient.println("<br><br>");      //Saut de lignes
    //sprintf(tmpChaine,"<form method=\"get\" action=\"updateSsid?id=%d \"  >", index);
    sprintf(tmpChaine,"<form method=\"get\" action=\"updateSsid\"  >");
    Serial.println(tmpChaine);
    wifiClient.println(tmpChaine);  
    wifiClient.println("<input");
    wifiClient.println("   type=\"number\" ");
    wifiClient.println("   name=\"id\" ");
    wifiClient.println("   value=");
    wifiClient.println(    index);
    wifiClient.println("/>");
    wifiClient.println("<table>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <th>SSID</th>");
    wifiClient.println("        <th>Passwd</th>");
    wifiClient.println("    </tr>");
    wifiClient.println("    <tr>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"string\" ");
    wifiClient.println("                name=\"ssid\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     getSsid(index));
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("        <td>");
    wifiClient.println("            <input");
    wifiClient.println("                type=\"string\" ");
    wifiClient.println("                name=\"pwd\" ");
    wifiClient.println("                value=");
    wifiClient.println(                     getPwd(index));
    wifiClient.println("            />");
    wifiClient.println("        </td>");
    wifiClient.println("    </tr>");
    wifiClient.println("</table>");
    wifiClient.println("<input type=\"submit\" value=\"Valider\">");
    wifiClient.println("</form>");
    wifiClient.println("<br><br>");      //Saut de lignes
    wifiClient.println("<br><a href= \" / \" >Retour</a><br>");
    wifiClient.println("<br><br>");      //Saut de lignes
}

//=========================================
//
//          updateSsid
//
//=========================================
void updateSsid(String request){
    Serial.printf("updateSsid\n");
    String tmp;
    String ssid;
    String pwd;
    String idx;
    int index1 = request.lastIndexOf("?");
    int index2 = request.lastIndexOf(" HTTP");
    tmp=request.substring(index1+1,index2);
    do{
        String item = tmp.substring(0,tmp.indexOf('&'));
        //Serial.print("mise a jour de ");
        //Serial.println(item);
        int index = item.indexOf('=');
        String name = item.substring(0,index);
        String value = item.substring(index+1,item.length());
        if (name == "id"){
            idx = value;
        }
        if (name == "ssid"){
            ssid = value;
        }
        tmp=tmp.substring(tmp.indexOf('&')+1, tmp.length());
    } while (tmp.indexOf('&') != -1);
    int index = tmp.indexOf('=');
    String name = tmp.substring(0,index);
    String value = tmp.substring(index+1,tmp.length());
    if (name == "pwd"){
        pwd = value;
    }
    setSsid(idx.toInt(), ssid, pwd);
}

//=========================================
//
//          updateAccesPoint
//
//=========================================
void updateAccesPoint(String request){
    char tmp[50];
    char type[10];
    int index1 = request.lastIndexOf("?");
    int index2 = request.lastIndexOf(" HTTP");
    request.substring(22,index1).toCharArray(type,10);
    Serial.print("type de requete : ");
    Serial.println(type);
    index1 = request.lastIndexOf("=");
    request.substring(index1+1,index2).toCharArray(tmp,50);
    Serial.printf("action sur acces point : <%s>\n", tmp);
    if (strcmp(type,"up") == 0){
        accesPointUp(atoi(tmp));
        displayAccesPoints();
    } else if (strcmp(type,"down") == 0){
        accesPointdown(atoi(tmp));
        displayAccesPoints();
    } else if (strcmp(type,"edit") == 0){
        accesPointEdit(atoi(tmp));
    } else {
        Serial.printf("type inconnu <%s>", type);
    }
}

//=========================================
//
//          analyseRequest
//
//=========================================
void analyseRequest(String request){

    Serial.print("Requete a analyser : ");
    Serial.print(request);
    Serial.println();
    displayHeader();
    if (request.equals("GET / HTTP/1.1")){                  // requete principale
        displayHomeMessage();
    }else if (request.equals("GET /alarme HTTP/1.1")){      // requetes de gestion d'alarme
        displayConfigAlarme();
    }else if (request.startsWith("GET /updateAlarme")){
        update(request);
        displayConfigAlarme();
    }else if (request.startsWith("GET /switchReveil")){
        switchReveilOnOff();
        displayConfigAlarme();
    }else if (request.equals("GET /heure HTTP/1.1")){      // requetes de gestion de l'heure
        displayConfigHeure();
    }else if (request.startsWith("GET /updateHeure")){
        update(request);
        displayConfigHeure();
    }else if (request.startsWith("GET /updateNTP")){
        refreshNtpNow();
        displayConfigHeure();
    }else if (request.startsWith("GET /save")){
        saveDatasToFlash();
        displayHomeMessage();
    }else if (request.startsWith("GET /accesPoint")){      // requetes de gestion des points d'acces wifi
        displayAccesPoints();
    }else if (request.startsWith("GET /updateAccesPoint/updateSsid")){
        updateSsid(request);
        displayAccesPoints();
    }else if (request.startsWith("GET /updateAccesPoint")){
        updateAccesPoint(request);
    } else {                                                // requetes erreur
        displayErrorScreen();
    }
    displayFooter();
}
