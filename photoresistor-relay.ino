/* Use a photoresistor (or photocell) to turn on an LED in the dark
   More info and circuit schematic: http://www.ardumotive.com/how-to-use-a-photoresistor-en.html
   Dev: Michalis Vasilakis // Date: 8/6/2015 // www.ardumotive.com */
   

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//Constants
const int analogPin = A0;  // Analog pin connected to your sensor
const int threshold = 1000;   // Threshold value for triggering action
const unsigned long durationThreshold = 10000;  // 10 seconds threshold

unsigned long startTime = 0; // Variable to store the start time
bool aboveThreshold = false;  // Flag to indicate if the value is above threshold

const int ledPin=9;       // Led pin at Arduino pin 9

void setup(){
 pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(analogPin, INPUT);// Set pResistor - A0 pin as an input (optional)
 Serial.begin(9600); // open the serial port at 9600 bps:

  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(5);     
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);     
}

void loop(){
  int sensorValue = analogRead(analogPin); // Read the analog value
  bool currentValueAboveThreshold = (sensorValue > threshold); // Check if current value is above threshold

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(sensorValue);
  display.display();
  delay(500); // Pause for 2 seconds

   Serial.print(sensorValue);
   Serial.println();

  if (currentValueAboveThreshold && !aboveThreshold) {
    // Transition from below to above threshold
    startTime = millis(); // Record start time
    aboveThreshold = true; // Update flag
  }

  if (!currentValueAboveThreshold && aboveThreshold) {
    // Transition from above to below threshold
    aboveThreshold = false; // Update flag
  }

  if (aboveThreshold && (millis() - startTime >= durationThreshold)) {
    // If the value has been above threshold for 10 seconds
    digitalWrite(ledPin, LOW);  //Turn led off
  }
  else {
    digitalWrite(ledPin, HIGH); //Turn led on
  }

  delay(500); //Small delay
}
