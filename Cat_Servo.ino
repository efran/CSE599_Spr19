// Modeled after: https://electronut.in/a-simple-python-to-arduino-alert-scheme/
// Also refered to: https://www.intorobotics.com/tutorial-how-to-control-the-tower-pro-sg90-servo-with-arduino-uno/

// ardu_alert.ino
//
// read serial port and turn leds on/off
//
// Mahesh Venkitachalam
// electronut.in


#include "Arduino.h"
#include "Servo.h"

// Instead of LEDs, I want the the servo motor to activate --
// But I could ALSO have some LEDs :-)
// LED pin numbers (digital)

int servoPin = 9;
Servo servo;  
int servoAngle = 0;   // servo position in degrees
bool active = false;

//int pinRed = 4;
//int pinGreen = 2;

void setup()
{
  // initialize serial comms
  Serial.begin(9600); 

  // set pins 
  servo.attach(servoPin);
  //pinMode(pinRed, OUTPUT);
  //pinMode(pinGreen, OUTPUT);
}

void loop()
{
  // Python will need to send a signal to the Arduino -- either 0 or 1
  while(Serial.available() > 0) { 
      
    int c = Serial.read();

    if (c == '1') {
      if (!active) {
        active = true;
        // Activate servo here -- have it remain activated for 30 seconds
        servo.write(45);      // Turn SG90 servo Left to 45 degrees
        delay(1000);          // Wait 1 second
        servo.write(90);      // Turn SG90 servo back to 90 degrees (center position)
        delay(1000);          // Wait 1 second
        servo.write(135);     // Turn SG90 servo Right to 135 degrees
        delay(1000);          // Wait 1 second
        servo.write(90);      // Turn SG90 servo back to 90 degrees (center position)
        delay(1000);
        //digitalWrite(pinRed, LOW);
        //digitalWrite(pinGreen, HIGH);
        active = false;
      }
    }
    //else if (c == '0') {
    //  digitalWrite(pinRed, HIGH);
    //  digitalWrite(pinGreen, LOW);
    //}
    //else {
    //  digitalWrite(pinRed, LOW);
    //  digitalWrite(pinGreen, LOW);
    //}
  }
  
  // wait 
  delay(100);
}
