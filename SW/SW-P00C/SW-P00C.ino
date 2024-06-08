// HW Version v1.0 

#include <Adafruit_NeoPixel.h>
#include "matrix.h"

// Pin where the LED strip is connected
#define LED_PIN 6

// Pins where the buttons are connected
#define BUTTON_PIN 4
#define BRIGHTNESS_BUTTON_PIN 5
#define NUM_LEDS_LIT_BUTTON_PIN 7
#define COLOR_BUTTON_PIN 8
#define CLEAR_BUTTON_PIN 9

// Number of LEDs in the strip
#define NUM_LEDS 96

// Array of possible numbers of LEDs to light up at a time
const int numLEDsLitOptions[] = {1, 2, 3, 4};
const int numNumLEDsLitOptions = sizeof(numLEDsLitOptions) / sizeof(numLEDsLitOptions[0]);
int currentNumLEDsLitIndex = 0;

int numLEDsLit = numLEDsLitOptions[currentNumLEDsLitIndex];

static int startIndex = 0;

// Brightness levels
const int brightnessLevels[] = {10, 30, 50, 80, 100};
const int numBrightnessLevels = sizeof(brightnessLevels) / sizeof(brightnessLevels[0]);
int currentBrightnessIndex = 0;

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Colors
const uint32_t colors[] = {
  strip.Color(20, 0, 0), // Red
  strip.Color(0, 20, 0), // Green
  strip.Color(0, 0, 20)  // Blue
};
const int numColors = sizeof(colors) / sizeof(colors[0]);
int currentColorIndex = 0;

// Function to set a specific LED in the strip
void setLED(int row, int col, uint32_t color) {
  if (row >= 0 && row < rows && col >= 0 && col < cols) {
    int ledIndex = matrix[row][col];
    strip.setPixelColor(ledIndex, color);
  }
}

// Function to light up LEDs based on the numLEDsLit variable and start position
void lightUpMatrix(int startIndex) {
  // Turn off all LEDs initially
  strip.clear();
  
  int count = 0;

  // Calculate the starting row and column based on startIndex
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int currentIndex = i * cols + j;
      if (currentIndex >= startIndex && count < numLEDsLit) {
        setLED(i, j, colors[currentColorIndex]); // Set each LED to the current color
        count++;
      }
    }
  }
  strip.show();
}

// Function to set brightness
void setBrightness() {
  uint8_t brightness = map(brightnessLevels[currentBrightnessIndex], 0, 100, 0, 255);
  strip.setBrightness(brightness);
  strip.show(); // Update the strip with the new brightness
}

void setup() {
  // Initialize the LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Initialize the button pins as input with internal pull-up resistors
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NUM_LEDS_LIT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CLEAR_BUTTON_PIN, INPUT_PULLUP);

  // Set initial brightness
  setBrightness();

  // Initial light up matrix
  lightUpMatrix(startIndex);
}

void loop() {
  static bool buttonPressed = false;
  static bool brightnessButtonPressed = false;
  static bool numLEDsLitButtonPressed = false;
  static bool colorButtonPressed = false;
  static bool clearButtonPressed = false;

  // Check if the main button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Debounce the button press
    if (!buttonPressed) {
      buttonPressed = true;

      // Light up the matrix starting from the current startIndex
      lightUpMatrix(startIndex);

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
  if (digitalRead(BRIGHTNESS_BUTTON_PIN) == LOW) {
    // Debounce the brightness button press
    if (!brightnessButtonPressed) {
      brightnessButtonPressed = true;

      // Advance to the next brightness level
      currentBrightnessIndex = (currentBrightnessIndex + 1) % numBrightnessLevels;
      
      // Set the new brightness immediately
      setBrightness();
    }
  } else {
    brightnessButtonPressed = false;
  }

  // Check if the numLEDsLit button is pressed
  if (digitalRead(NUM_LEDS_LIT_BUTTON_PIN) == LOW) {
    // Debounce the numLEDsLit button press
    if (!numLEDsLitButtonPressed) {
      numLEDsLitButtonPressed = true;

      // Advance to the next numLEDsLit option
      currentNumLEDsLitIndex = (currentNumLEDsLitIndex + 1) % numNumLEDsLitOptions;
      numLEDsLit = numLEDsLitOptions[currentNumLEDsLitIndex];

      // Update the matrix immediately
      lightUpMatrix(startIndex);
    }
  } else {
    numLEDsLitButtonPressed = false;
  }

  // Check if the color button is pressed
  if (digitalRead(COLOR_BUTTON_PIN) == LOW) {
    // Debounce the color button press
    if (!colorButtonPressed) {
      colorButtonPressed = true;

      // Advance to the next color
      currentColorIndex = (currentColorIndex + 1) % numColors;

      // Update the matrix immediately
      lightUpMatrix(startIndex);
    }
  } else {
    colorButtonPressed = false;
  }

  // Check if the clear button is pressed
  if (digitalRead(CLEAR_BUTTON_PIN) == LOW) {
    // Debounce the clear button press
    if (!clearButtonPressed) {
      clearButtonPressed = true;

      // Clear the matrix and reset startIndex
      startIndex = 0;
      strip.clear();
      strip.show();
    }
  } else {
    clearButtonPressed = false;
  }
}
