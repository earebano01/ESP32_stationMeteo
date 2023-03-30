// #include <Arduino.h>
// #include <WiFi.h>
// #include <Wire.h>
// #include <Adafruit_BME280.h>
// #include <Adafruit_Sensor.h>

// unsigned long delayTime;

// #define SEALEVELPRESSURE_HPA (1013.25)

// Adafruit_BME280 bme; // I2C
// //Adafruit_BME280 bme(BME_CS); // hardware SPI
// //Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// // Replace with your network credentials
// // const char* ssid = "BELL209";
// // const char* password = "65E14F2C5217";
// const char* ssid = "UNIFI_IDO1";
// const char* password = "42Bidules!";

// // Set web server port number to 80
// WiFiServer server(80);

// // Variable to store the HTTP request
// String header;

// // Current time
// unsigned long currentTime = millis();
// // Previous time
// unsigned long previousTime = 0; 
// // Define timeout time in milliseconds (example: 2000ms = 2s)
// const long timeoutTime = 2000;

// void printValues() {
//   Serial.print("Temperature = ");
//   Serial.print(bme.readTemperature());
//   Serial.println(" *C");
  
//   // Convert temperature to Fahrenheit
//   /*Serial.print("Temperature = ");
//   Serial.print(1.8 * bme.readTemperature() + 32);
//   Serial.println(" *F");*/
  
//   Serial.print("Pressure = ");
//   Serial.print(bme.readPressure() / 100.0F);
//   Serial.println(" hPa");

//   Serial.print("Altitude = ");
//   Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//   Serial.println(" m");

//   Serial.print("Humidity = ");
//   Serial.print(bme.readHumidity());
//   Serial.println(" %");

//   Serial.println();
//   delayTime = 3000;
// }

// void setup() {
//   Serial.begin(9600);
//   bool status;

//   // default settings
//   // (you can also pass in a Wire library object like &Wire2)
//   //status = bme.begin();  
//   if (!bme.begin(0x76)) {
//     Serial.println("Could not find a valid BME280 sensor, check wiring!");
//     while (1);
//   }

//   // Connect to Wi-Fi network with SSID and password
//   Serial.print("Connecting to ");
//   Serial.println(ssid);
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   // Print local IP address and start web server
//   Serial.println("");
//   Serial.println("WiFi connected.");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
//   server.begin();
// }

// void loop(){
//   printValues();
//   delay(delayTime);

//   WiFiClient client = server.available();   // Listen for incoming clients

//   if (client) {                             // If a new client connects,
//     currentTime = millis();
//     previousTime = currentTime;
//     Serial.println("New Client.");          // print a message out in the serial port
//     String currentLine = "";                // make a String to hold incoming data from the client
//     while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
//       currentTime = millis();
//       if (client.available()) {             // if there's bytes to read from the client,
//         char c = client.read();             // read a byte, then
//         Serial.write(c);                    // print it out the serial monitor
//         header += c;
//         if (c == '\n') {                    // if the byte is a newline character
//           // if the current line is blank, you got two newline characters in a row.
//           // that's the end of the client HTTP request, so send a response:
//           if (currentLine.length() == 0) {
//             // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//             // and a content-type so the client knows what's coming, then a blank line:
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type:text/html");
//             client.println("Connection: close");
//             client.println();
            
//             // // Display the HTML web page
//             client.println("<!DOCTYPE html><html>");
//             client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//             client.println("<link rel=\"icon\" href=\"data:,\">");

//             // CSS to style the table 
//             client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
//             client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
//             client.println("th { padding: 12px; background-color: #0043af; color: white; }");
//             client.println("tr { border: 1px solid #ddd; padding: 12px; }");
//             client.println("tr:hover { background-color: #bcbcbc; }");
//             client.println("td { border: none; padding: 12px; }");
//             client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
//             // Web Page Heading
//             client.println("</style></head><body><h1>ESP32 with BME280</h1>");
//             client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
//             client.println("<tr><td>Temperature Celsius</td><td><span class=\"tempC\">");
//             client.println(bme.readTemperature());
//             client.println(" *C</span></td></tr>");  
//             client.println("<tr><td>Temperature Fahrenheit</td><td><span class=\"tempF\">");
//             client.println(1.8 * bme.readTemperature() + 32);
//             client.println(" *F</span></td></tr>");       
//             client.println("<tr><td>Pressure</td><td><span class=\"press\">");
//             client.println(bme.readPressure() / 100.0F);
//             client.println(" hPa</span></td></tr>");
//             client.println("<tr><td>Altitude</td><td><span class=\"alt\">");
//             client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
//             client.println(" m</span></td></tr>"); 
//             client.println("<tr><td>Humidity</td><td><span class=\"hum\">");
//             client.println(bme.readHumidity());
//             client.println(" %</span></td></tr>"); 
//             client.println("</body></html>");

//             // client.println("HTTP/1.1 200 OK");
//             // client.println("Content-type:text/html");
//             // client.println("Connection: close");
//             // client.println();
//             // client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\"><style>");
//             // client.println("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\">");
//             // client.println("<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.3/font/bootstrap-icons.css\">");
//             // client.println("<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js\"></script>");
//             // client.println(".border {border: solid black solid 2px;width: 622px;border-radius: 5px;}");
//             // client.println(".container2 {background-color: rgb(170, 167, 167, 0.4);}");
//             // client.println(".temperature {color:#eb9a00;}");
//             // client.println(".humidity {color:#1f9cdc;}");
//             // client.println(".pressure {color:#1a8d43;}");
//             // client.println(".altitude {color:#6a3eb1;}");
//             // client.println(".padding {padding: 10px;}");
//             // client.println(".padding2 {padding-left: 20px;padding-right: 20px;padding-top: 20px;padding-bottom: 20px;}");
//             // client.println(".padding3 {padding-left: 10px;padding-right: 10px;}");
//             // client.println("</style></head><body>");
//             // client.println("<div class=\"border padding3 text-center position-absolute top-50 start-50 translate-middle\">");
//             // client.println("<div class=\"row padding container2\">");
//             // client.println("<h2>ESP8622 Weather Station</h2></div>");
//             // client.println("<div class=\"row padding\">");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"72\" height=\"72\" fill=\"#eb9a00\" class=\"bi bi-thermometer\" viewBox=\"0 0 16 16\">");
//             // client.println("<path d=\"M8 14a1.5 1.5 0 1 0 0-3 1.5 1.5 0 0 0 0 3z\"/>");
//             // client.println("<path d=\"M8 0a2.5 2.5 0 0 0-2.5 2.5v7.55a3.5 3.5 0 1 0 5 0V2.5A2.5 2.5 0 0 0 8 0zM6.5 2.5a1.5 1.5 0 1 1 3 0v7.987l.167.15a2.5 2.5 0 1 1-3.333 0l.166-.15V2.5z\"/></svg></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><h2>Temperature Celsius</h2></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding temperature\"><h1><span class=\"tempC\">");
//             // client.println(bme.readTemperature());
//             // client.println("*C</span></h1></div></div>");
//             // client.println("<div class=\"row padding\"><div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"72\" height=\"72\" fill=\"#eb9a00\" class=\"bi bi-thermometer\" viewBox=\"0 0 16 16\">");
//             // client.println("<path d=\"M8 14a1.5 1.5 0 1 0 0-3 1.5 1.5 0 0 0 0 3z\"/>");
//             // client.println("<path d=\"M8 0a2.5 2.5 0 0 0-2.5 2.5v7.55a3.5 3.5 0 1 0 5 0V2.5A2.5 2.5 0 0 0 8 0zM6.5 2.5a1.5 1.5 0 1 1 3 0v7.987l.167.15a2.5 2.5 0 1 1-3.333 0l.166-.15V2.5z\"/></svg></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><h2>Temperature Fahrenheit</h2></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding temperature\"><h1><span class=\"tempF\">");
//             // client.println(1.8 * bme.readTemperature() + 32);
//             // client.println("</span></h1></div></div>");
//             // client.println("<div class=\"row padding\"><div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"72\" height=\"72\" fill=\"#1f9cdc\" class=\"bi bi-droplet-fill\" viewBox=\"0 0 16 16\">");
//             // client.println("<path d=\"M8 16a6 6 0 0 0 6-6c0-1.655-1.122-2.904-2.432-4.362C10.254 4.176 8.75 2.503 8 0c0 0-6 5.686-6 10a6 6 0 0 0 6 6ZM6.646 4.646l.708.708c-.29.29-1.128 1.311-1.907 2.87l-.894-.448c.82-1.641 1.717-2.753 2.093-3.13Z\"/></svg></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><h2>Humidity</h2></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding humidity\"><h3><span class=\"hum\">");
//             // client.println(bme.readHumidity());
//             // client.println("</span></h3></div></div>");
//             // client.println("<div class=\"row padding\"><div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"72\" height=\"72\" fill=\"#1a8d43\" class=\"bi bi-speedometer\" viewBox=\"0 0 16 16\">");
//             // client.println("<path d=\"M8 2a.5.5 0 0 1 .5.5V4a.5.5 0 0 1-1 0V2.5A.5.5 0 0 1 8 2zM3.732 3.732a.5.5 0 0 1 .707 0l.915.914a.5.5 0 1 1-.708.708l-.914-.915a.5.5 0 0 1 0-.707zM2 8a.5.5 0 0 1 .5-.5h1.586a.5.5 0 0 1 0 1h2.5A.5.5 0 0 1 2 8zm9.5 0a.5.5 0 0 1 .5-.5h1.5a.5.5 0 0 1 0 1H12a.5.5 0 0 1-.5-.5zm.754-4.246a.389.389 0 0 0-.527-.02L7.547 7.31A.91.91 0 1 0 8.85 8.569l3.434-4.297a.389.389 0 0 0-.029-.518z\"/>");
//             // client.println("<path fill-rule=\"evenodd\" d=\"M6.664 15.889A8 8 0 1 1 9.336.11a8 8 0 0 1-2.672 15.78zm-4.665-4.283A11.945 11.945 0 0 1 8 10c2.186 0 4.236.585 6.001 1.606a7 7 0 1 0-12.002 0z\"/></svg></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><h2>Pressure</h2></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding pressure\"><h3><span class=\"press\">");
//             // client.println(bme.readPressure() / 100.0F);
//             // client.println("hPa</span></h3></div></div>");
//             // client.println("<div class=\"row padding\"><div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><svg id=\"Layer_1\" data-name=\"Layer 1\" xmlns=\"http://www.w3.org/2000/svg\" width=\"72\" height=\"72\" fill=\"#9158ee\" viewBox=\"0 0 122.88 78.87\"><defs><style>.cls-1{fill-rule:evenodd;}</style></defs><title>mountains</title><path class=\"cls-1\" d=\"M90.4,28.29l.08.24.22.62.08.24L91,30l.08.24.19.56.08.23.18.53.08.23.17.5.08.23L92,33l.07.23.15.44.08.23.14.4.08.23.13.38.08.22.12.35.07.22L93,36l.07.21.11.31.07.21.1.29.07.2.09.26.07.2.09.25.07.19.08.23.06.18.07.21.06.18.07.2.06.17.07.18,0,.17.06.16.06.16,0,.15,0,.16,0,.14.05.15,0,.12,0,.15,0,.11,0,.14,0,.1,0,.13,0,.1,0,.12,0,.09,0,.12,0,.08,0,.11,0,.07,0,.1,0,.07,0,.1,0,.06,0,.09v.06l0,.09,0,0,0,.08v0l0,.09,0,.11v.25h0l0,.18v.5h0V44h0v.08h0v.29h0v.35h0v.13h0v.13h0v.15h0v.16h0v.17h0v.18h0V46h0v.22h0v.24c.07,5,6.36,11,5.2,14.44s4.68,9.9,6.56,14.71H98.63a3.31,3.31,0,0,0-.6-1.91L79.93,44.76l3-4.85,3.65-3.66,3.69-8.1h0l.08.14ZM92.19,27l30.28,48.39a2.17,2.17,0,0,1,.41,1.28,2.21,2.21,0,0,1-2.21,2.21H3.28A3.28,3.28,0,0,1,.53,73.79L47.26,1.66A3.16,3.16,0,0,1,48.37.5,3.29,3.29,0,0,1,52.9,1.55L78.46,42.41l10-15.35a2.07,2.07,0,0,1,.75-.78,2.2,2.2,0,0,1,3,.71ZM52.44,75.58H3.9L14.33,61.79l16-28L39.47,23.2l4.65-10.28,6-9.64h0l.12.2c.88,2.66,1.26,4.11,2,6.25.44,1.23,2.3,3.91,2.67,4.93,5.31,14.76,2.62,9.34,2.69,15.88.08,7.51,9.47,16.45,7.74,21.53-1.93,5.62,8.38,16.22,10.31,23.51Z\"/></svg></div>");
//             // client.println("<div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding\"><h2>Altitude</h2></div><div class=\"col-lg-4 col-md-4 col-sm-4 col-xs-4 padding altitude\"><h3><span class=\"alt\">");
//             // client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
//             // client.println("m</span></h3></div>");
//             // client.println("</div></div></body></html>");
      
            
//             // The HTTP response ends with another blank line
//             client.println();
//             // Break out of the while loop
//             break;
//           } else { // if you got a newline, then clear currentLine
//             currentLine = "";
//           }
//         } else if (c != '\r') {  // if you got anything else but a carriage return character,
//           currentLine += c;      // add it to the end of the currentLine
//         }
//       }
//     }
//     // Clear the header variable
//     header = "";
//     // Close the connection
//     client.stop();
//     Serial.println("Client disconnected.");
//     Serial.println("");
//   }
// }

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
unsigned long timerDelay = 10000;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

float temperature;
float humidity;
float pressure;

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
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
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

JSONVar readings;

String getSensorReadings(){
  readings["tempC"] = String(bme.readTemperature());
  readings["tempF"] = String(1.8 * bme.readTemperature() + 32);
  readings["press"] =  String(bme.readPressure() / 100.0F);
  readings["alt"] =  String(bme.readAltitude(SEALEVELPRESSURE_HPA));
  readings["hum"] =  String(bme.readHumidity());
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP Weather Station</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
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
            background-color: #006d47;
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
    <title>ESP Weather Station</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
</head>
<body>
    <h1>ESP32 with BME280</h1>
    <table>
        <tr>
            <th>MEASUREMENT</th>
            <th>VALUE</th>
        </tr>
        <tr>
            <td>Temperature Celsius</td>
            <td><span id="tempC"></span></td>
        </tr>
        <tr>
            <td>Temperature Fahrenheit</td>
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
      window.addEventListener('load', getReadings);

  function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      var tempC = myObj.tempC;
      var tempF = myObj.tempF;
      var press = myObj.press;
      var alt = myObj.alt;
      var hum = myObj.hum;
      gaugeTemp.value = temp;
      gaugeHum.value = hum;
    }
  }; 
  xhr.open("GET", "/readings", true);
  xhr.send();
}

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
  
  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    gaugeTemp.value = myObj.temperature;
    gaugeHum.value = myObj.humidity;
  }, false);
}
</script>
</body>
</html>
)rawliteral";

void setup(void) {
  Serial.begin(9600);

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(200, "text/plain", "Hi! I am ESP32.");
  // });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  printValues();
  delay(delayTime);

  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();
  }
}