// ESP32_horloge
// serverWeb.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <Wifi.h>
#include "wifiTools.hpp"
#include "reveil.hpp"
#include "heure.hpp"
#include "ntp.hpp"
#include "eeprom.hpp"

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
    wifiClient.println("<head><title>Horloge ESP 32</title>");   //Pour l'onglet du navigateur
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
//          analyseRequest
//
//=========================================
void analyseRequest(String request){

    //Serial.print("Requete a analyser : ");
    //Serial.print(request);
    //Serial.println();
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
    } else {                                                // requetes erreur
        displayErrorScreen();
    }
    displayFooter();
}
