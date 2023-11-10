#include "Ultrasonic.h"

// Define the pins for the ultrasonic sensor
const int trigPin = 5;
const int echoPin = 4;
const int buzzer = 2;
const int LED = 0;

#define BAUDRATE 115200

// Variables to store the duration and distance
long duration;
int distance;

void setup() {
 // Initialize serial communication
 Serial.begin(115200);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(buzzer, OUTPUT);
 pinMode(LED, OUTPUT);
}
void loop() {
 // Trigger the ultrasonic sensor to send a pulse
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 
 // Measure the duration of the pulse on the echoPin
 duration = pulseIn(echoPin, HIGH);
 
 // Calculate the distance based on the speed of sound (343 meters/second)
 distance = duration * 0.0343 / 2;
 
 // Print the distance to the Serial Monitor
 Serial.print("Distance: ");
 Serial.print(distance);
 Serial.println(" cm");

 if(distance <20){
  digitalWrite(buzzer,HIGH);
  digitalWrite(LED,HIGH);
 } 
 else{
  digitalWrite(buzzer,LOW);
  digitalWrite(LED,LOW);
 }
 
 // Wait for a short time before taking the next measurement
 delay(550);
}

void testLED(){
  digitalWrite(13, HIGH); 
  delay(1000);            
  digitalWrite(13, LOW);  
  delay(1000);            
}
