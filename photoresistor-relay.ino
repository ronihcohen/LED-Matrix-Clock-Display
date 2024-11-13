/* Use a photoresistor (or photocell) to turn on an relay with a screen */

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

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(sensorValue);
  display.display();
  delay(500);

  Serial.print(sensorValue);
  Serial.println();

  if (sensorValue > 1000) {
    digitalWrite(ledPin, HIGH);
  }

  if (sensorValue > 900 && sensorValue < 950) {
    digitalWrite(ledPin, LOW);
  }

  delay(500);
}
