// HW Version v1.0 

#include <Adafruit_NeoPixel.h>

// Pin where the LED strip is connected
#define LED_PIN 6

// Number of LEDs in the strip
#define NUM_LEDS 96

// Number of LEDs to light up at a time
int numLEDsLit = 3;

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Define the dimensions of the matrix
const int rows = 8;
const int cols = 12;

// Initialize the 3x4 matrix with positions corresponding to the LED strip
int matrix[rows][cols] = 
{
  {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
  {83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72},
  {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71},
  {59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48},
  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
  {35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24},
  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
  {11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0}
};

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
}

void loop() {
  static int startIndex = 0;

  // Light up the matrix starting from the current startIndex
  lightUpMatrix(startIndex);

  // Advance the startIndex to the next position
  startIndex += numLEDsLit;
  if (startIndex >= NUM_LEDS) {
    startIndex = 0; // Reset to the beginning
  }

  // Wait for 5 seconds before advancing to the next set of LEDs
  delay(2000);
}