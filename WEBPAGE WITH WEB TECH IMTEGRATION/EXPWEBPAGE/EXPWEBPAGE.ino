
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage_exp.h"
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 18

#include <Adafruit_GPS.h>
#define GPSSerial Serial2

Adafruit_GPS GPS(&GPSSerial);

#define GPSECHO false

uint32_t timer = millis();

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

String getLongi(){
  float longi_int = float(GPS.longitude);
  longi_int=longi_int/100;
  String longi= String(longi_int);
  return longi;
}

String getLati(){
  float lati_int = float(GPS.latitude);
  lati_int=lati_int/100;
  String lati = String(lati_int);
  return lati;
}
String getGoogle(){
  String googleMapsLink = "https://www.google.com/maps?q=";
  googleMapsLink += String(GPS.latitude/100, 2);
  googleMapsLink += ",";
  googleMapsLink += String(-GPS.longitude/100, 2);
  return googleMapsLink;
}

void handleRoot() {
  String message = homePagePart1 + getLongi() + homePagePart2 +getLati() + homePagePart3+"<a href='" + getGoogle()+ "'>"+ homePagePart4+"</a>";
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
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
 
  server.begin();
  Serial.println("HTTP server started");

  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // GPS
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic parsing test!");
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
}
 
void loop(void) {
    // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
   // Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude/100, 2); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(-GPS.longitude/100, 2); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);
    }
  }
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
