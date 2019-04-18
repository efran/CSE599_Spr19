
  // Emilia Gan
  // CSE 599 Prototyping Interactive Systems
  // Assignment 1: Interactive Night Light
  // April 18, 2019

  /* This code controls an interactive teddy bear nightlight.
   * The nightlight contains an RGB LED that responds to ambient light. The 
   * photoresistor is located in the bear's belly button.
   * The color of the LED can be changed using a home-made low-fi pressure 
   * sensor in the bear's right paw.
   * The bear's left paw contains a commercial FSR that acts to turn on the
   * vibrating motor in the bear's nose and the green LEDs behind the bear's eyes.
   * Demo available: https://www.youtube.com/watch?v=zl_usvlVTuY
   */

  // Declarations
  const int RGB_RED = 3;
  const int RGB_GREEN = 5;
  const int RGB_BLUE= 6;
  const int GREEN_LED = 9;      // controls 2 green LEDs in series
  const int VIB = 10;
  const int PHOTO = A0;
  const int PRESSURE = A1;
  const int LOWFI = A3;

  int lowfiCounter = 0;         // variable for determining which color to change in RGB LED
  int red;                      // variable for setting red value
  int green;                    // variable for setting green value
  int blue;                     // variable for setting blue value
  boolean changeColor = false;  // variable to determine when to change color
  int pressureInput = 0;        // variable to hold mapped FSR pressure read
  int lowfiInput = 0;           // variable to hold mapped home-made low-fi sensor read

  // FUNCTIONS
  
  /*
   * Function to get input from the photoresistor
   * and return a value mapped from 0 to 1.
   * The photoresistor reads in a value whose magnitude is
   * directly proportional to the amount of light detected.
   * This function maps the output so that bright
   * ambient light returns a value of 1. 
   */
float photoRead() {
  int inPhotoVal = analogRead(PHOTO); // raw detected light read
  delay(10);
  int photoVal;                       // variable to hold mapped value

  if (inPhotoVal >= 40) {photoVal = 100;}
  else {photoVal = map(inPhotoVal, 0, 40, 0, 100);}
  float brightnessFactor = photoVal/100.0; // Bright light returns brightnessFactor of 1

      // For troubleshooting
    Serial.print("Raw photo reading: ");
    Serial.print(inPhotoVal);
    Serial.print(", Mapped photo value: ");
    Serial.println(photoVal);

    Serial.print("Brightness Factor: ");
    Serial.println(brightnessFactor);

  return brightnessFactor;
}

/*
 * Function to get input from the FSR pressure resistor
 * and return a mapped value ranging from 1023 to 300 
 * (not mapping down to zero intentionaaly, as the vibrations 
 * are too intense).
 */
int pressureRead() {
  int inPressureVal = analogRead(PRESSURE); // get raw read
  delay(10);
  int pressureVal;                          // variable to hold mapped value

  if (inPressureVal >= 900) {pressureVal = 300;}
  else {pressureVal = map(inPressureVal, 0, 900, 1023, 300);}

  // For troubleshooting
  Serial.print("Raw pressure reading: ");
  Serial.print(inPressureVal);
  Serial.print(", Mapped pressure value: ");
  Serial.println(pressureVal);

  return pressureVal;
}

/*
 * Function to get input from the homemade low-fi pressure sensor/potentiometer
 * and map it to a number in the range 0 to 255 (suitable for use in setting
 * the RGB LED values).
 */
int lowfiRead() {
  int inLowfiVal = analogRead(LOWFI); // raw low-fi sensor input
  delay(10);
  int lowfiVal;                       // variable to hold mapped value

  if (inLowfiVal < 100) {
    changeColor = false;
    lowfiVal = 255;
    }
  else {
    changeColor = true;
    lowfiVal = map(inLowfiVal, 100, 300, 255, 0);
    }

   // For troubleshooting
   Serial.print("Raw low-fi sensor reading: ");
   Serial.print(inLowfiVal);
   Serial.print(", Mapped low-fi  sensor value: ");
   Serial.println(lowfiVal);

   return lowfiVal;
  
}

/* Function to set the LED values of the RGB LED. The function
 * require 3 int settings in the range 0 - 255 to set the red, 
 * green, and blue values. The brigtnessFactor input, which ranges from 0 to 1 (generally
 * the output from the photoRead() function), then adjusts the in-
 * put red, green, and blue values proportionally. A brightness factor of 0 will 
 * not change the input values, whereas a brightness factor of 1 will set all the 
 * LEDs to 255 (i.e. the off state).
 */

void rgbWriteAdj(int r, int g, int b, float brightnessFactor) {
  // brightnessFactor ranges from 0 to 1

    r = r + (255 - r) * brightnessFactor;
    g = g + (255 -g) * brightnessFactor;
    b = b + (255 - b) * brightnessFactor;

    analogWrite(RGB_RED, r);
    analogWrite(RGB_GREEN, g);
    analogWrite(RGB_BLUE, b);
}

  

void setup() {
  // put your setup code here, to run once:
  
  // LEDs
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Vibration Motor
  pinMode(VIB, OUTPUT);

  // Light and (2) Pressure Sensors
  pinMode(PHOTO, INPUT);
  pinMode(PRESSURE, INPUT);
  pinMode(LOWFI, INPUT);

  Serial.begin(9600);

  // Set initial RGB LED color
  red = 0;
  green = 255;
  blue = 255;

  
}


  void loop() {
  // put your main code here, to run repeatedly:

  // Reset lowfiCounter, if needed:
  if (lowfiCounter == 3) {lowfiCounter = 0;}

  // Initially, turn off green LEDs in bear's eyes
  analogWrite(GREEN_LED, 255);

  // initially, this will use the values from the set-up
  rgbWriteAdj(red, green, blue, photoRead());

  // If FSR sensor detects pressure, vibrate nose and turn on eyes.
  pressureInput = pressureRead();
  delay(10);
  analogWrite(VIB, pressureInput);
  if (pressureInput < 1000) {
    analogWrite(GREEN_LED, 0);
  }

  // If low-fi sensor detects input, change one of the RGB LED colors, depending 
  // on the current value of lowfiCounter.
  lowfiInput = lowfiRead();
  if (changeColor == true) {
    if (lowfiCounter == 0) {
      red = lowfiInput;
    }
    else {
      if (lowfiCounter == 1) {
        green = lowfiInput;
      }
      else { blue = lowfiInput;}
    }
    lowfiCounter = lowfiCounter + 1;
    rgbWriteAdj(red, green, blue, photoRead());
  }
 

  //for troubleshooting
  Serial.print("RED: ");
  Serial.print(red);
  Serial.print(" ~ GREEN: ");
  Serial.print(green);
  Serial.print(" ~ BLUE: ");
  Serial.println(blue);
 
}
