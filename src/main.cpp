#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Arduino_JSON.h>

// Replace with your network credentials
const char* ssid = "BELL209";
const char* password = "65E14F2C5217";

// const char* ssid = "UNIFI_IDO1";
// const char* password = "42Bidules!";

unsigned long delayTime;

unsigned long lastTime = 0;
unsigned long timerDelay = 500;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

float temperatureC;
float temperatureF;
float pressure;
float altitude;
float humidity;

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

AsyncWebServer server(80);
AsyncEventSource events("/events");

// Init BME280
void initBME(){
    if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void getSensorReadings(){
  temperatureC = bme.readTemperature();
  temperatureF = 1.8 * bme.readTemperature() + 32;
  pressure = bme.readPressure()/ 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity = bme.readHumidity();
}

// Initialize WiFi
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

String processor(const String& var){
  getSensorReadings();
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return String(temperatureC);
  }
  else if(var == "TEMPERATUREF"){
    return String(temperatureF);
  }
  else if(var == "PRESSURE"){
    return String(pressure);
  }
  else if(var == "ALTITUDE"){
    return String(altitude);
  }
  else if(var == "HUMIDITY"){
    return String(humidity);
  }
  return String();
}

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

void setup(void) {
  Serial.begin(9600);
  initWiFi();
  initBME();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  // printValues();
  // delay(delayTime);

  if ((millis() - lastTime) > timerDelay) {
    getSensorReadings();
    Serial.printf("Temperature Celsius = %.2f ºC \n", temperatureC);
    Serial.printf("Temperature Farenheit = %.2f ºF \n", temperatureF);
    Serial.printf("Pressure = %.2f hPa \n", pressure);
    Serial.printf("Altitude = %.2f m \n", altitude);
    Serial.printf("Humidity = %.2f \n", humidity);
    Serial.println();

    // Send Events to the Web Client with the Sensor Readings
    events.send("ping",NULL,millis());
    events.send(String(temperatureC).c_str(),"temperatureC",millis());
    events.send(String(temperatureF).c_str(),"temperatureF",millis());
    events.send(String(pressure).c_str(),"pressure",millis());
    events.send(String(altitude).c_str(),"altitude",millis());
    events.send(String(humidity).c_str(),"humidity",millis());
    
    lastTime = millis();
  }
}