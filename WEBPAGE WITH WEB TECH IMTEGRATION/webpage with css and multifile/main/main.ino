//THINGSPEAK AND DHT11

#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 18
#define REPORTING_PERIOD_MS 20000 //report to thingspeak every 20s

uint32_t timer = millis();
const char* hs_ssid = "LaPhone";
const char* hs_password = "password";
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include "about.h"
#include "demos.h"
#include "features.h"
#include "dht11.h"
#include "gps.h"

String myStatus;
float temp;
float humi;
 
WebServer server(80);
 uint32_t tsLastReport = 0; //4 byte unsigned int to time thingspeak 20s

//temp function to simulate temp sensor
int getTemp() {
  int temp_int = DHT.temperature;
  return temp_int;
}
String getHumi(){
  String humi_string = String(DHT.humidity);
  return humi_string;
}

void handleRoot() {
Serial.println("GET /about");
  server.send(200, "text/html", htmlAbout);
}
 void handleDemos() {
Serial.println("GET /demos");
  server.send(200, "text/html", htmlDemos);
}
void handleFeatures() {
Serial.println("GET /features");
  server.send(200, "text/html", htmlFeatures);
}
void handleDHT11() {
Serial.println("GET /dht11");
String message = htmlDHT11 + homePagePart1 + String(getTemp()) + homePagePart2 +getHumi() + homePagePart3;
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void) {
  Serial.begin(115200);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  WiFi.begin(hs_ssid, hs_password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(hs_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
 
  server.on("/", handleRoot);
  server.on("/demos", handleDemos);
  server.on("/features", handleFeatures);
  server.on("/dht11", handleDHT11);
  server.on("/inline", []() {
  server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
 
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {

  ThingSpeak.setField(1,temp);
  ThingSpeak.setField(2,humi);

  DHT.read(DHT11_PIN);
  temp = DHT.temperature;
  Serial.print("Temp:");
  Serial.println(temp);
  humi = DHT.humidity;
  Serial.print("Humi:");
  Serial.println(humi);
  delay(2000);
  
  server.handleClient();
   if (millis () - tsLastReport > REPORTING_PERIOD_MS)
 {
  ThingSpeak.setStatus(myStatus);
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  tsLastReport = millis();
  //delay(2);//allow the cpu to switch to other tasks
  }
}
