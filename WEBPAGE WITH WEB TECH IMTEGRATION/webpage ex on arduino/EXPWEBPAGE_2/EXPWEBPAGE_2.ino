#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 18

#include "dht11_2.h"
#include "homepage_exp_2.h"
#include "about_2.h"

const char* ssid = "LaPhone";
const char* password = "password";
 
WebServer server(80);
 
//temp function to simulate temp sensor
String getTemp() {
  DHT.read(DHT11_PIN);
  String temp = String(DHT.temperature);
  return temp;
}
String getHumi(){
  String humi = String(DHT.humidity);
  return humi;
}
 
 
void handleRoot() {
Serial.println("GET /");
  server.send(200, "text/html", htmlHomePage_2);
}
 void handleDHT11_2() {
Serial.println("GET /dht11_2");
  server.send(200, "text/html", htmlDHT11_2);
}
void handleAbout_2() {
Serial.println("GET /about_2");
  server.send(200, "text/html", htmlAbout_2);
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
 
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
 
  server.on("/", handleRoot);
  server.on("/dht11_2", handleDHT11_2);
  server.on("/about_2", handleAbout_2);

  server.on("/inline", []() {
  server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
 
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
