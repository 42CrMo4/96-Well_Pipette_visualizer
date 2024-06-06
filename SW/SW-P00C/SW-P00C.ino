/*
 * LED Matrix Lighting Program
 * HW Version: v1.0
 *
 * This program controls a strip of 96 NeoPixel LEDs arranged in an 8x12 matrix.
 *
 * The program utilizes the Adafruit_NeoPixel library to manage the LED strip.
 * 
 * Hardware Setup:
 * - The LED strip is connected to pin 6 of the microcontroller.
 * - The matrix is defined as an 8x12 grid, mapping the LED indices to their positions.
 *
 * Dependencies:
 * - Adafruit_NeoPixel library
 *
 * Created by 42CrMo4, 2024
 */

#include <Adafruit_NeoPixel.h>
#include "matrix.h"

// Pin where the LED strip is connected
#define LED_PIN 6

// Pin where the button is connected
#define BUTTON_PIN 4

// Number of LEDs in the strip
#define NUM_LEDS 96

// Number of LEDs to light up at a time
int numLEDsLit = 3;

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
void lightUpMatrix(int startIndex) {
  // Turn off all LEDs initially
  strip.clear();
  
  int count = 0;
  
  // Calculate the starting row and column based on startIndex
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int currentIndex = i * cols + j;
      if (currentIndex >= startIndex && count < numLEDsLit) {
        setLED(i, j, strip.Color(20, 0, 0)); // Set each LED to red color
        count++;
      }
    }
  }
  strip.show();
}

void setup() {
  // Initialize the LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Initialize the button pin as input with an internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize serial communication for UART simulation
  Serial.begin(9600);
}

void loop() {
  static int startIndex = 0;
  static bool buttonPressed = false;

  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW || (Serial.available() && Serial.read() == 'p')) {
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
}