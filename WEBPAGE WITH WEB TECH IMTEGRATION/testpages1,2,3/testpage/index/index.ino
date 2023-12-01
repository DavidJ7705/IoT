#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "page1.h"
#include "page2.h"
#include "page3.h"

const char* ssid = "LaPhone";
const char* password = "password";
 
WebServer server(80);

void handleRoot() {
  Serial.println("GET /");
  server.send(200, "text/html", htmlPage1);
}

void handlePage2() {
  Serial.println("GET /page2");
  server.send(200, "text/html", htmlPage2);
}

void handlePage3() {
  Serial.println("GET /page3");
  server.send(200, "text/html", htmlPage3);
}

void setup(void){
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

  server.on("/", handleRoot);
  server.on("/page2", handlePage2);
  server.on("/page3", handlePage3);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}