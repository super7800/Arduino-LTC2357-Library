#include <Arduino.h>
#include "LTC2357.h"

// Define the pin connections
const uint8_t CS_PIN = 10;   // Chip Select pin for the LTC2348
const uint8_t CNV_PIN = 9;   // Convert Start pin for the LTC2348
const uint8_t BUSY_PIN = 8;  // Busy pin from the LTC2348

// Create an instance of the LTC2348 class
LTC2357 adc(CS_PIN, CNV_PIN, BUSY_PIN);

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Initialize the LTC2348 device
  adc.begin();

  Serial.println("ADC Initialized");
}

void loop() {
  // Get ADC values
  float* adc_values = adc.get_adc_values();

  // Print the ADC values to the Serial Monitor
  for (int i = 0; i < 4; i++) {
    Serial.print("Channel ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(adc_values[i], 6);  // Print with 6 decimal places for precision
  }

  Serial.println("-------------------");

  // Delay for a second before reading again
  delay(1000);
}