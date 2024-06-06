// HW Version v1.0 

#include <Adafruit_NeoPixel.h>
#include "matrix.h"

// Pin where the LED strip is connected
#define LED_PIN 6

// Pins where the buttons are connected
#define BUTTON_PIN 4
#define BRIGHTNESS_BUTTON_PIN 5

// Number of LEDs in the strip
#define NUM_LEDS 96

// Number of LEDs to light up at a time
int numLEDsLit = 3;

// Brightness levels
const int brightnessLevels[] = {10, 30, 50, 80, 100};
const int numBrightnessLevels = sizeof(brightnessLevels) / sizeof(brightnessLevels[0]);
int currentBrightnessIndex = 0;

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Function to set a specific LED in the strip
void setLED(int row, int col, uint32_t color) {
  if (row >= 0 && row < rows && col >= 0 && col < cols) {
    int ledIndex = matrix[row][col];
    strip.setPixelColor(ledIndex, color);
  }
}

// Function to light up LEDs based on the numLEDsLit variable and start position
void lightUpMatrix(int startIndex, int brightness) {
  // Turn off all LEDs initially
  strip.clear();
  
  int count = 0;
  

  // Calculate the starting row and column based on startIndex
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int currentIndex = i * cols + j;
      if (currentIndex >= startIndex && count < numLEDsLit) {
        setLED(i, j, strip.Color(brightness, 0, 0)); // Set each LED to red color
        count++;
      }
    }
  }
  strip.setBrightness(brightness);
  strip.show();
}

void setup() {
  // Initialize the LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Initialize the button pins as input with internal pull-up resistors
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON_PIN, INPUT_PULLUP);

  // Initialize serial communication for UART simulation
  Serial.begin(9600);
}

void loop() {
  static int startIndex = 0;
  static bool buttonPressed = false;
  static bool brightnessButtonPressed = false;

  // Check if the main button is pressed
  if (digitalRead(BUTTON_PIN) == LOW || (Serial.available() && Serial.read() == 'p')) {
    // Debounce the button press
    if (!buttonPressed) {
      buttonPressed = true;

      // Light up the matrix starting from the current startIndex
      int brightness = map(brightnessLevels[currentBrightnessIndex], 0, 100, 0, 255);
      lightUpMatrix(startIndex, brightness);

      // Advance the startIndex to the next position
      startIndex += numLEDsLit;
      if (startIndex >= NUM_LEDS) {
        startIndex = 0; // Reset to the beginning
      }
    }
  } else {
    buttonPressed = false;
  }

  // Check if the brightness button is pressed
  if (digitalRead(BRIGHTNESS_BUTTON_PIN) == LOW || (Serial.available() && Serial.read() == 'b')) {
    // Debounce the brightness button press
    if (!brightnessButtonPressed) {
      brightnessButtonPressed = true;

      // Advance to the next brightness level
      currentBrightnessIndex = (currentBrightnessIndex + 1) % numBrightnessLevels;

      int brightness = map(brightnessLevels[currentBrightnessIndex], 0, 100, 0, 255);
      lightUpMatrix(startIndex, brightness);
    }
  } else {
    brightnessButtonPressed = false;
  }
}
