#include "Ultrasonic.h"

// Define the pins for the ultrasonic sensor
const int SW = 25;
const int buzzer = 2;
const int LED = 0;

#define BAUDRATE 115200
// Variables to store the duration and distance

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  pinMode(SW, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
     //int value;
    
    //value = digitalRead( SW );

    if (digitalRead(SW)==HIGH){
        digitalWrite(LED,LOW);
        digitalWrite(buzzer,LOW);
    }
    else{
        digitalWrite(LED,HIGH);
        digitalWrite(buzzer,HIGH);
    }
  // Wait for a short time before taking the next measurement
  delay(200);
}
