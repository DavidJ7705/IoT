// Define the pins for the ultrasonic sensor
const int trigPin_1 = 15;
const int echoPin_1 = 13;

const int trigPin_2 = 5;
const int echoPin_2 = 4;

const int buzzer = 2;
const int LED = 0;

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 18

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
PulseOximeter pox;

#include <Adafruit_GPS.h>
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false

uint32_t timer = millis();
#define BAUDRATE 115200
#define REPORTING_PERIOD_MS 1000

// Variables to store the duration and distance
long duration_1;
int distance_1;

long duration_2;
int distance_2;

float temp;
float humi;

unsigned long previousMillis = 0;
const long interval = 2000; // Interval in milliseconds

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);
    
	// Configure sensor to use 7.6mA for LED drive
	pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);

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

void loop() {
   pox.update();
  unsigned long currentMillis = millis();
 
  // Check if it's time to read sensors
  if (currentMillis - previousMillis >= interval) {
    // Save the last time we read the sensors
    previousMillis = currentMillis;

    // Read heart rate sensor
      Serial.print("\nHeart rate:");
      Serial.print(pox.getHeartRate());
      Serial.print("bpm / SpO2:");
      Serial.print(pox.getSpO2());
      Serial.println("%");
  
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
    if (GPS.newNMEAreceived()) {
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
        Serial.print("Angle: "); Serial.println(GPS.angle);
        Serial.print("Altitude: "); Serial.println(GPS.altitude);
        Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      }
  }
}

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

void onBeatDetected()
{
    Serial.println("Beat!");
}