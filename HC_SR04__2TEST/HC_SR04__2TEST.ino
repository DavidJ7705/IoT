#include "Ultrasonic.h"

// Define the pins for the ultrasonic sensor

const int trigPin_2 = 17;
const int echoPin_2 = 16;

const int buzzer = 2;
const int LED = 0;

#define BAUDRATE 115200

// Variables to store the duration and distance
long duration_2;
int distance_2;

void setup() {
 // Initialize serial communication
 Serial.begin(115200);
 pinMode(trigPin_2, OUTPUT);
 pinMode(echoPin_2, INPUT);
 pinMode(buzzer, OUTPUT);
 pinMode(LED, OUTPUT);
}
void loop() {
 // Trigger the ultrasonic sensor to send a pulse


 digitalWrite(trigPin_2, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin_2, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin_2, LOW);
 
 // Measure the duration of the pulse on the echoPin
 duration_2 = pulseIn(echoPin_2, HIGH);
 
 // Calculate the distance based on the speed of sound (343 meters/second)
 distance_2 = duration_2 * 0.0343 / 2;
 
 // Print the distance to the Serial Monitor

 Serial.print("Distance 2: ");
 Serial.print(distance_2);
 Serial.println(" cm");



 //ultrasonic 2
  if(distance_2<100 && distance_2 > 50){
  digitalWrite(buzzer,HIGH);
  digitalWrite(LED,HIGH);
  delay(600);
  digitalWrite(buzzer,LOW);
  digitalWrite(LED,LOW);
  delay(100);
 }
 
 else if(distance_2<35 && distance_2 > 20){
  digitalWrite(buzzer,HIGH);
  digitalWrite(LED,HIGH);
  delay(30);
  digitalWrite(buzzer,LOW);
  digitalWrite(LED,LOW);
  delay(50);
  }
 else if(distance_2 <20){
  digitalWrite(buzzer,HIGH);
  digitalWrite(LED,HIGH);
 } 
 else{
  digitalWrite(buzzer,LOW);
  digitalWrite(LED,LOW);
 }
 
 // Wait for a short time before taking the next measurement
 delay(450);
}



void testBuzzer(){
  tone(buzzer, 1000, 2000);
  delay(2000);
  tone(buzzer, 100, 2000);
  delay(2000);
}

void testLED(){
  digitalWrite(13, HIGH); 
  delay(1000);            
  digitalWrite(13, LOW);  
  delay(1000);            
}
