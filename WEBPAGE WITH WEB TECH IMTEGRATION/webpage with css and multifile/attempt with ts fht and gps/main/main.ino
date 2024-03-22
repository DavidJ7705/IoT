//libraries and header files needed
#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

//dht11 files
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 18


//gps files
#include <Adafruit_GPS.h>
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false


#define BAUDRATE 115200
#define REPORTING_PERIOD_MS 20000 //report to thingspeak every 20s
uint32_t timer = millis();

// Variables to store the duration and distance for ultrasonic sensors
long duration_1;
int distance_1; //for ultrasonic q

long duration_2;
int distance_2; //for ultrasonic 2

//variables to store dht11 readings
float temp;
float humi;

//variables for adding gps longitude and latitude to thingspeak
int lati_ts;
int longi_ts;

//for looping the main every 2 seconds
unsigned long previousMillis = 0; //execute the loop in void main only if the current time is - the last time readings were taken is greater than 2 seconds
const long interval = 2000; // Interval in milliseconds


//ssis and password for connecting esp32 to a network
const char* hs_ssid = "LaPhone"; //hot spot phone name  
const char* hs_password = "password"; //hot spot phone password
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

//needed for thingspeak
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

//different header files relating to the different sections of my webpage
#include "about.h"
#include "demos.h"
#include "features.h"
#include "dht11.h"
#include "gps.h"

// Define the pins for the ultrasonic sensor
const int trigPin_1 = 15;
const int echoPin_1 = 13; //for ultrasonic 1, labeled with blue on board

const int trigPin_2 = 5;
const int echoPin_2 = 4; //for ultrasonic 2, labeled with red on board

const int buzzer = 2;
const int LED = 0;

String myStatus;  //setting status on thingspeak(not used)
 
WebServer server(80);
 uint32_t tsLastReport = 0; //4 byte unsigned int to time thingspeak 20s

//Functions for passing dht11 readings into webpage using strings
int getTemp() {//type int as i was practicing typecasting 
  int temp_int = DHT.temperature;
  return temp_int;
}
String getHumi(){
  String humi_string = String(DHT.humidity);
  return humi_string;
}

//Making a tring out of longitude and latitude
String getLongi(){
  float longi_v1;// divide 100
  int longi_int;//takes out the integer
  float longi_dec;//takes out the decimal
  double longi_v2;//final longitude

  longi_v1 = GPS.longitude/100;
  longi_int = longi_v1;
  longi_dec = longi_v1 - longi_int;
  longi_dec = longi_dec/0.6;
  longi_v2 = longi_int + longi_dec;

  String longi = String(-longi_v2,2);
  return longi;
}

String getLati(){
  float lati_v1;// divide 100
  int lati_int;//takes out the integer
  float lati_dec;//takes out the decimal
  double lati_v2;//final latitude
  String lati;

  lati_v1 = GPS.latitude/100;
  lati_int = lati_v1;
  lati_dec = lati_v1 - lati_int;
  lati_dec = lati_dec/0.6;
  lati_v2 = lati_int + lati_dec;

  lati = String(lati_v2,2);
  return lati;
}

//String to get the Google maps location to a 5 metre tolerance, 
//on the webpage we are able to click a link that takes us to the location on googlemaps
String getGoogle(){
  float lati_v1;// divide 100
  int lati_int;//takes out the integer
  float lati_dec;//takes out the decimal
  double lati_v2;//final latitude

  lati_v1 = GPS.latitude/100;
  lati_int = lati_v1;
  lati_dec = lati_v1 - lati_int;
  lati_dec = lati_dec/0.6;
  lati_v2 = lati_int + lati_dec;

  float longi_v1;// divide 100
  int longi_int;//takes out the integer
  float longi_dec;//takes out the decimal
  double longi_v2;//final longitude

  longi_v1 = GPS.longitude/100;
  longi_int = longi_v1;
  longi_dec = longi_v1 - longi_int;
  longi_dec = longi_dec/0.6;
  longi_v2 = longi_int + longi_dec;
  
  //we are concatenating the latitude and longitude to the https, so that we can access the link on google
  String googleMapsLink = "https://www.google.com/maps?q=";
  googleMapsLink += String(lati_v2,6);
  googleMapsLink += ",";
  googleMapsLink += String(-longi_v2,6);
  return googleMapsLink;
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
String message = htmlDHT11 + dht_homePagePart1 + String(getTemp()) + dht_homePagePart2 +getHumi() + dht_homePagePart3;
  server.send(200, "text/html", message);
}
void handleGPS(){
  Serial.println("GET /gps");

  //setting these new variables = to the functions above,
  //so we can add them to the iframe link i created.
  //this will let us view the map location on the webpage without clicking the link
  String lat = getLati();
  String lon = getLongi();

  String iframe = "<center> <iframe width=\"350\" height=\"250\" frameborder=\"0\" style=\"border:0\" referrerpolicy=\"no-referrer-when-downgrade\" src=\"https://www.google.com/maps/embed/v1/place?key=AIzaSyAn7COqk8b29M1GINRhM_fWCwLQpPkTsbI&zoom=14&q=" + lat + "," + lon + "\" allowfullscreen></iframe></center>";

  String message = htmlGPS + gps_homePagePart1 + getLati() + gps_homePagePart2 + getLongi() + gps_homePagePart3 + "<a href='" + getGoogle() + "'>" + gps_homePagePart4 + "</a><br><br>" + iframe +"<br><br>"+ gps_homePagePart5;
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
  server.on("/gps", handleGPS);
  server.on("/inline", []() {
  server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
 
  server.begin();
  Serial.println("HTTP server started");

//configuring the different pins on the components as inputs or outputs
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(LED, OUTPUT);

  // GPS
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
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time we read the sensors
    previousMillis = currentMillis;
    
    // Read DHT11 sensor
    DHT.read(DHT11_PIN);
    temp = DHT.temperature;
    Serial.print("\nTemp:");
    Serial.println(temp);
    humi = DHT.humidity;
    Serial.print("Humi:");
    Serial.println(humi);
    
    // Measure distance for sensor 1
    ultraSonic(trigPin_1, echoPin_1, duration_1, distance_1);
    // Print the distance to the Serial Monitor
    Serial.print("\nDistance 1: ");
    Serial.print(distance_1);
    Serial.println(" cm");

    // Measure distance for sensor 2
    ultraSonic(trigPin_2, echoPin_2, duration_2, distance_2);
    // Print the distance to the Serial Monitor
    Serial.print("Distance 2: ");
    Serial.print(distance_2);
    Serial.println(" cm");

    // Control buzzer and LED
    controlBuzzer(distance_1, distance_2);
  }

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

      //thingspeak values for longitude and latitude
      lati_ts = GPS.latitude/100;
      longi_ts = -GPS.longitude/100;

    }
  }
  

  server.handleClient();
   if (millis () - tsLastReport > REPORTING_PERIOD_MS)
 {
  ThingSpeak.setStatus(myStatus);
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  ThingSpeak.setField(1,temp);
  ThingSpeak.setField(2,humi);
  ThingSpeak.setField(3,lati_ts);
  ThingSpeak.setField(4,longi_ts);

  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  tsLastReport = millis();
  }
}

//fucntion allows for the 2 ultrasonics to be able to be read
void ultraSonic(int trig, int echo, long &duration, int &distance) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.0343 / 2;
}



void controlBuzzer(int dist_1, int dist_2) {
  if (dist_1 < 30 || dist_2 < 30) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(LED, LOW);
  }
}