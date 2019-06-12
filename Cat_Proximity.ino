/*
 HC-SR04 Ping distance sensor]
 VCC to arduino 5v GND to arduino GND
 Echo to Arduino pin 13 Trig to Arduino pin 12
 More info at:
 https://www.winkleink.com/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 Also referred to the following for writing out to Python:
 https://www.instructables.com/id/Interface-Python-and-Arduino-with-pySerial/
 */

 // This code reportedly gives decent readings out to 70 cm

#define trigPin 11
#define echoPin 12
int num = 0;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);
  delay(500);
}
