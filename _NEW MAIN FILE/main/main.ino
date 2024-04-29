//PROGRAM TO TEST HEART RATE SENSOR AND ASYNCRONOUS DHT11
//2
//main file
//libraries and header files needed
#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h"  // always include thingspeak header file after other header files and custom macros
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>


//different header files relating to the different sections of my webpage
#include "about.h"
#include "demos.h"
#include "features.h"
#include "dht11.h"
#include "gps.h"
#include "heart.h"
#include "ultrasonic.h"


//asynchronous dht11
#include <Arduino.h>
#include "DHT_Async.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 19;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
float temperature;
float humidity;

//heart rate sensor
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
PulseOximeter pox;
float beat;


//gps files
#include <Adafruit_GPS.h>
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false


#define BAUDRATE 115200
#define REPORTING_PERIOD_MS 15000            //report to thingspeak every 20s
#define REPORTING_PERIOD_MS_THINGSPEAK 1000  // report to ThingSpeak every 20s

// Variables to store the duration and distance for ultrasonic sensors
long duration_1;
int distance_1;  //for ultrasonic 1

long duration_2;
int distance_2;  //for ultrasonic 2

//variables to store dht11 readings
float temp;
float humi;

//variables for adding gps longitude and latitude to thingspeak
int lati_ts;
int longi_ts;

//for looping the main every 2 seconds
unsigned long previousMillis = 0;  //execute the loop in void main only if the current time is - the last time readings were taken is greater than 2 seconds
const long interval = 1000;        // Interval in milliseconds
uint32_t timer = millis();

//ssis and password for connecting esp32 to a network
const char* hs_ssid = "LaPhone";       //hot spot phone name
const char* hs_password = "password";  //hot spot phone password
char ssid[] = SECRET_SSID;             // your network SSID (name)
char pass[] = SECRET_PASS;             // your network password
int keyIndex = 0;                      // your network key Index number (needed only for WEP)
WiFiClient client;
WebServer server(80);


//needed for thingspeak
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;
uint32_t tsLastReport = 0;            //4 byte unsigned int to time thingspeak 20s
uint32_t tsLastReportThingSpeak = 0;  //4 byte unsigned int to to time ThingSpeak 20s

// Define the pins for the ultrasonic sensor
const int trigPin_1 = 15;
const int echoPin_1 = 4;  //for ultrasonic 1, labeled with blue on board 4

const int trigPin_2 = 5;
const int echoPin_2 = 25;  //for ultrasonic 2, labeled with red on board 13

const int buzzer = 2;
const int LED = 0;

//String myStatus;  //setting status on thingspeak(not used)

//Functions for passing dht11 readings into webpage using strings

int getTemp() {  //type int as i was practicing typecasting
                 // int temperature = DHT.temperature;
  return temperature;
}
String getHumi() {
  //String humidity = String(DHT.humidity);
  return String(humidity);
}

String getDist_1() {
  return String(distance_1);
}

int getDist_2() {
  return distance_2;
}

String getHeart(){
  return String(beat);
}
String getSPo2(){
  return String(pox.getSpO2());
}


//Making a tring out of longitude and latitude
String getLongi() {
  float longi_v1;   // divide 100
  int longi_int;    //takes out the integer
  float longi_dec;  //takes out the decimal
  double longi_v2;  //final longitude

  longi_v1 = GPS.longitude / 100;
  longi_int = longi_v1;
  longi_dec = longi_v1 - longi_int;
  longi_dec = longi_dec / 0.6;
  longi_v2 = longi_int + longi_dec;

  String longi = String(-longi_v2, 6);
  return longi;
}

String getLati() {
  float lati_v1;   // divide 100
  int lati_int;    //takes out the integer
  float lati_dec;  //takes out the decimal
  double lati_v2;  //final latitude
  String lati;

  lati_v1 = GPS.latitude / 100;
  lati_int = lati_v1;
  lati_dec = lati_v1 - lati_int;
  lati_dec = lati_dec / 0.6;
  lati_v2 = lati_int + lati_dec;

  lati = String(lati_v2, 6);
  return lati;
}

//String to get the Google maps location to a 5 metre tolerance,
//on the webpage we are able to click a link that takes us to the location on googlemaps
String getGoogle() {
  float lati_v1;   // divide 100
  int lati_int;    //takes out the integer
  float lati_dec;  //takes out the decimal
  double lati_v2;  //final latitude

  lati_v1 = GPS.latitude / 100;
  lati_int = lati_v1;
  lati_dec = lati_v1 - lati_int;
  lati_dec = lati_dec / 0.6;
  lati_v2 = lati_int + lati_dec;

  float longi_v1;   // divide 100
  int longi_int;    //takes out the integer
  float longi_dec;  //takes out the decimal
  double longi_v2;  //final longitude

  longi_v1 = GPS.longitude / 100;
  longi_int = longi_v1;
  longi_dec = longi_v1 - longi_int;
  longi_dec = longi_dec / 0.6;
  longi_v2 = longi_int + longi_dec;

  //we are concatenating the latitude and longitude to the https, so that we can access the link on google
  String googleMapsLink = "https://www.google.com/maps?q=";
  googleMapsLink += String(lati_v2, 6);
  googleMapsLink += ",";
  googleMapsLink += String(-longi_v2, 6);
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
  String message = htmlDHT11 + dht_homePagePart1 + String(getTemp()) + dht_homePagePart2 + getHumi() + dht_homePagePart3;
  server.send(200, "text/html", message);
}


void handleGPS() {
  Serial.println("GET /gps");

  //setting these new variables = to the functions above,
  //so we can add them to the iframe link i created.
  //this will let us view the map location on the webpage without clicking the link
  String lat = getLati();
  String lon = getLongi();
  String iframe = "<center> <iframe width=\"450\" height=\"350\" frameborder=\"1\" style=\"border:1\" referrerpolicy=\"no-referrer-when-downgrade\" src=\"https://www.google.com/maps/embed/v1/place?key=AIzaSyAn7COqk8b29M1GINRhM_fWCwLQpPkTsbI&zoom=14&q=" + lat + "," + lon + "\" allowfullscreen></iframe></center>";

  String message = htmlGPS + gps_homePagePart1 + getLati() + gps_homePagePart2 + getLongi() + gps_homePagePart3 + "<a href='" + getGoogle() + "'>" + gps_homePagePart4 + "</a><br><br>" + iframe + "<br><br>" + gps_homePagePart5;
  server.send(200, "text/html", message);
}


void handleHEART() {
  Serial.println("GET /heart");
  String message = htmlHEART + heart_homePagePart1 + String(getHeart()) + heart_homePagePart2 + String(getSPo2()) + heart_homePagePart3;
  server.send(200, "text/html", message);
}


void handleULTRASONIC() {
  Serial.println("GET /ultrasonic");
  String message = htmlULTRASONIC + ultrasonic_homePagePart1 + getDist_1() + ultrasonic_homePagePart2 + String(getDist_2()) + ultrasonic_homePagePart3;
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

  delay(1000);
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
  server.on("/heart", handleHEART);
  server.on("/ultrasonic", handleULTRASONIC);
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
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);


  //heart rate sensor inititation
  // Register a callback for the beat detection
  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment(float* temperature, float* humidity) {
  static unsigned long measurement_timestamp = millis();

  /* Measure once every two seconds. */
  if (millis() - measurement_timestamp > 2000ul) {
    if (dht_sensor.measure(temperature, humidity)) {
      measurement_timestamp = millis();
      return (true);
    }
  }

  return (false);
}



void loop(void) {
  unsigned long currentMillis = millis();

  // Make sure to call update as fast as possible
  server.handleClient();
  pox.update();


  if (millis() - tsLastReportThingSpeak > REPORTING_PERIOD_MS_THINGSPEAK) {
    beat = pox.getHeartRate();


    tsLastReportThingSpeak = millis();  //update the time stamp
  }

  if (currentMillis - previousMillis >= interval) {
    // Save the last time we read the sensors
    previousMillis = currentMillis;

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
    if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
      return;                        // we can fail to parse a sentence in which case we should just wait for another
  }
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {

    // Measure distance for sensor 1
    ultraSonic(trigPin_1, echoPin_1, duration_1, distance_1);
    // Print the distance to the Serial Monitor
    Serial.print("\nDistance 1: ");
    Serial.print(distance_1);
    Serial.print(" cm  /  ");

    // Measure distance for sensor 2
    ultraSonic(trigPin_2, echoPin_2, duration_2, distance_2);
    // Print the distance to the Serial Monitor
    Serial.print("Distance 2: ");
    Serial.print(distance_2);
    Serial.println(" cm");


    timer = millis();  // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC);
    Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC);
    Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC);
    Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC);
    Serial.print('/');
    Serial.print(GPS.month, DEC);
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: ");
    Serial.print((int)GPS.fix);
    Serial.print(" quality: ");
    Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude / 100, 2);
      Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(-GPS.longitude / 100, 2);
      Serial.println(GPS.lon);
      //Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      //Serial.print("Angle: "); Serial.println(GPS.angle);
      // Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: ");
      Serial.println((int)GPS.satellites);

      //thingspeak values for longitude and latitude
      lati_ts = GPS.latitude / 100;
      longi_ts = -GPS.longitude / 100;
    }
  }


  if (measure_environment(&temperature, &humidity)) {
    Serial.print("\nTemp:");
    Serial.print(temperature, 1);
    Serial.print("  Humi:");
    Serial.print(humidity, 1);
    Serial.println("%");
  }

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, distance_1);
  ThingSpeak.setField(4, distance_2);
  ThingSpeak.setField(5, beat);

  updateTS();
}


//function updates thingspeak every 20 seconds
void updateTS() {
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    if (humidity >= 75) {
      Serial.println("\nRoads may be slippery!!\nProceed with caution.");
    }

    // pieces of information in a channel.  Here, we write to field all the fields.
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  //this ,ine is the problem with max30100

    if (!pox.begin()) {
      Serial.println("FAILED");
      for (;;)
        ;
    } else { 
      Serial.println("SUCCESS");
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
    pox.update();

    tsLastReport = millis();

  }
}


// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
  Serial.println("\nBeat!");
  Serial.print(beat);
  Serial.print("bpm / SpO2:");
  Serial.print(pox.getSpO2());
  Serial.println("%");
}


//fucntion allows for the 2 ultrasonics to be able to be read
void ultraSonic(int trig, int echo, long& duration, int& distance) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.0343 / 2;
}


//controls buzzer and LED if distance gathered is less than 30cm
void controlBuzzer(int dist_1, int dist_2) {
  if (dist_1 < 20 || dist_2 < 20) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(LED, HIGH);
    Serial.println("Object Detected");
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(LED, LOW);
  }
}