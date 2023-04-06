/*
Titre : Projet Final
Auteur: Adonis Rebano
Date : 06/04/2023
Description : Creation d'un objet intelligent avec une page web affichant les informations et envoi 
              des données en utilisant JSON
Version : 0.0.1

/*-----on faire notre declaration pour la librairie dont on besoin dans notre projet-----*/
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <SPIFFS.h>

#define SEALEVELPRESSURE_HPA (1013.25)          /* On utilise cette variable pour estimer l'altitude 
                                                   à partir d'une pression donnée en la comparant à 
                                                   la pression au niveau de la mer. */

/*-----on definit notre SSID et notre mot de passe------*/
const char* ssid = "BELL209";
const char* password = "65E14F2C5217";

// const char* ssid = "UNIFI_IDO1";
// const char* password = "42Bidules!";

/*-----on definit le delai different qu'on utilise dans notre projet*/
unsigned long delayTime;              
unsigned long lastTime = 0;
unsigned long timerDelay = 500;

Adafruit_BME280 bme;                             /* Puisqu'on utilise I2C, on a juste besoin de créer 
                                                    une classe et un objet */

/*-----on definit nos variables-----*/
float temperatureC;
float temperatureF;
float pressure;
float altitude;
float humidity;

/*-----on creé une fonctionne pour afficher des données sur le moniteur série-----*/
void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
  delayTime = 3000;
}

AsyncWebServer server(80);                      // on crée une AsyncWebServer objet sur le port 80
AsyncEventSource events("/events");             // on crée une AsyncEventSource sur /events 

/*-----on crée une fonctionne pour initialiser notre BME-----*/
void initBME(){
    if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

/*-----on crée une fonctionne pour obtenir des valeurs dans notre BME-----*/
void getSensorReadings(){
  temperatureC = bme.readTemperature();
  temperatureF = 1.8 * bme.readTemperature() + 32;
  pressure = bme.readPressure()/ 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity = bme.readHumidity();
}

/*-----on crée une fonctionne pour initialiser notre WiFi-----*/
void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

/*-----on crée la base pour notre HTML qui on peut modifier par l'OTA-----*/
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Station Meteo</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        body {
            text-align: center;
            font-family: "Trebuchet MS", Arial;
        }
        table {
            border-collapse: collapse;
            width: 35%;
            margin-left: auto;
            margin-right: auto;
        }
        th {
            padding: 12px;
            background-color: #c74a4a;
            color: white;
        }
        tr {
            border: 1px solid #ddd;
            padding: 12px;
        }
        tr:hover {
            background-color: #bcbcbc;
        }
        td {
            border: none;
            padding: 12px;
        }
        .sensor {
            color: white;
            font-weight: bold;
            background-color: #bcbcbc;
            padding: 1px;
        }
    </style>
</head>
<body>
    <h1>ESP32 Station Meteo</h1>
    <table>
        <tr>
            <th>MEASUREMENT</th>
            <th>VALUE</th>
        </tr>
        <tr>
            <td>Temperature (&#8451;)</td>
            <td><span id="tempC"></span></td>
        </tr>
        <tr>
            <td>Temperature (&#8457;)</td>
            <td><span id="tempF"></span></td>
        </tr>
        <tr>
            <td>Pressure</td>
            <td><span id="press"></span></td>
        </tr>
        <tr>
            <td>Altitude</td>
            <td><span id="alt"></span></td>
        </tr>
        <tr>
            <td>Humidity</td>
            <td><span id="hum"></span></td>
        </tr> 
      <script>
      if (!!window.EventSource) {
      var source = new EventSource('/events');
      
      source.addEventListener('open', function(e) {
        console.log("Events Connected");
      }, false);
      source.addEventListener('error', function(e) {
        if (e.target.readyState != EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      }, false);
      
      source.addEventListener('message', function(e) {
        console.log("message", e.data);
      }, false);
      
      source.addEventListener('temperatureC', function(e) {
        console.log("temperatureC", e.data);
        document.getElementById("tempC").innerHTML = e.data;
      }, false);

      source.addEventListener('temperatureF', function(e) {
        console.log("temperatureF", e.data);
        document.getElementById("tempF").innerHTML = e.data;
      }, false);
      
      source.addEventListener('pressure', function(e) {
        console.log("pressure", e.data);
        document.getElementById("press").innerHTML = e.data;
      }, false);

      source.addEventListener('altitude', function(e) {
        console.log("altitude", e.data);
        document.getElementById("alt").innerHTML = e.data;
      }, false);

      source.addEventListener('humidity', function(e) {
        console.log("humidity", e.data);
        document.getElementById("hum").innerHTML = e.data;
      }, false);
      
      }
</script>
</body>
</html>   
)rawliteral";

void setup() {
  Serial.begin(9600);                           //  on fait notre initialisation pour le moniteur série,
  initWiFi();                                   //  notre WiFi
  initBME();                                    //  et BME280

  // on gére les demandes des clients et on servir la page web sur la racine /
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // on gére des événements du serveur Web
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
  
  /* ensuite, on envoie un événement avec le message "hello !", l'ID de l'heure actuelle et définir 
     le délai de reconnexion à 1 seconde. */
    client->send("hello!", NULL, millis(), 10000);
  });

  AsyncElegantOTA.begin(&server);               // on demarre ElegantOTA, le serveur et notre handler
  server.begin();                               // pour l'événement
  server.addHandler(&events);                   

  Serial.println("HTTP server started");

  // on initialise notre SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.println("Mounted file system");

  // on servi des donnees JSON du SPIFFS et on peut acceder des donnes actuelle sur /data.json
  server.on("/data.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data.json", "application/json");
  });

}

void loop() {

  if ((millis() - lastTime) > timerDelay) {
    getSensorReadings();
    Serial.println();

    // on envoie des événements au client Web avec les lectures des capteurs getSensorReadings();.
    events.send("ping",NULL,millis());
    events.send(String(temperatureC).c_str(),"temperatureC",millis());
    events.send(String(temperatureF).c_str(),"temperatureF",millis());
    events.send(String(pressure).c_str(),"pressure",millis());
    events.send(String(altitude).c_str(),"altitude",millis());
    events.send(String(humidity).c_str(),"humidity",millis());
    
    lastTime = millis();
  }

  if (!isnan(temperatureC) && !isnan(temperatureF) && !isnan(pressure) && !isnan(altitude) && !isnan(humidity)) {
    printValues();
    
    // on enregistre les données des capteurs sur la mémoire SPIFFS dans le dossier qu'on a crée /data
    File file = SPIFFS.open("/data.json", FILE_WRITE);
    if (file) {
      file.print("{");
      file.print("\"temperatureC\":");
      file.print(temperatureC);
      file.print(",");
      file.print("\"temperatureF\":");
      file.print(temperatureF);
      file.print(",");
      file.print("\"pressure\":");
      file.print(pressure);
      file.print(",");
      file.print("\"altitude\":");
      file.print(altitude);
      file.print(",");
      file.print("\"humidity\":");
      file.print(humidity);
      file.print("}");
      file.close();
      Serial.println("Saved sensor data to SPIFFS");
    }
    else {
      Serial.println("Failed to open file for writing");
    }
  }
  else {
    Serial.println("Failed to read sensor data");
  }

  // on ajoute un delai d'une seconde
  delay(1000);
}