
// Define the pins for the ultrasonic sensor
const int trigPin_1 = 15;
const int echoPin_1 = 4;

const int trigPin_2 = 5;
const int echoPin_2 = 16;

const int buzzer = 2;
const int LED = 0;

#define BAUDRATE 115200

// Variables to store the duration and distance
long duration_1;
int distance_1;

long duration_2;
int distance_2;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Measure distance for sensor 1
  ultraSonic(trigPin_1, echoPin_1, duration_1, distance_1);
  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance_1);
  Serial.println(" cm");

  // Control buzzer and LED for sensor 1
 // controlBuzzer(distance);

  // Add a delay between sensor readings
  delay(200);

  // Measure distance for sensor 2
  ultraSonic(trigPin_2, echoPin_2, duration_2, distance_2);
  // Print the distance to the Serial Monitor
  Serial.print("Distance 2: ");
  Serial.print(distance_2);
  Serial.println(" cm");

  // Control buzzer and LED for sensor 2
  controlBuzzer(distance_1, distance_2);

  // Wait for a short time before taking the next measurement
  delay(200);
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
    delay(100);
  }
  else{
    digitalWrite(buzzer, LOW);
    digitalWrite(LED, LOW);
    delay(100);
  }
}
