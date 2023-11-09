#include "Ultrasonic.h"

#define TRIG_PIN 5
#define ECHO_PIN 4
#define buzzer 2

#define BAUDRATE 115200

Ultrasonic us(TRIG_PIN, ECHO_PIN);
float distance;
int startTime;
int sound = 0;

void setup() {
 pinMode(buzzer, OUTPUT);
 pinMode(TRIG_PIN, OUTPUT);
 pinMode(ECHO_PIN, INPUT);
 
 Serial.begin(115200);
 while(!Serial);

 startTime = millis();  
}

void loop() {


  us.measure();
  distance = millis() - startTime;
  distance /= 1000;
  
  Serial.println(us.get_cm(), 3);
/*
  if(distance >1000){
    noTone(buzzer);
  }
  else if(distance <40 && distance >20){
     tone(buzzer,500,1500);
  }
    else if(distance <30){
        tone(buzzer,1000,1000);
  }
*/

/*
 if(distance>=100){
  noTone(buzzer);
 }
 else if (distance <= 20) {
    tone(buzzer, 1000, 20);
    sound = 10;
  }
  else {
    noTone(buzzer);
  }
  */
  
  delay(50);
  
  
}

void testTone(){
  tone(buzzer, 1000, 2000);
  delay(2000);
  tone(buzzer, 100, 2000);
  delay(2000);
}
