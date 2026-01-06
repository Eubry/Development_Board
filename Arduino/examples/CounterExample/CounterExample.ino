/**
 * @file CounterExample.ino
 * @brief Example demonstrating the Counter library (cCount and cTime classes)
 * @version 1.0.0
 * @date 2026-01-05
 * @author Eubry Gomez Ramirez
 * 
 * This example demonstrates:
 * - Using cCount for event counting
 * - Using cTime for timing operations (with fallback for non-ESP32 platforms)
 * - Flag management with counters
 * - Callback functionality
 */

#include "counter.h"

// Create counter and timer instances
cCount buttonCounter;
cCount ledBlinker;
cTime intervalTimer;

// Flags for state management
bool currentState = false;
bool nextState = false;

// LED pin
const int LED_PIN = LED_BUILTIN;

// Variables for demonstration
int loopCount = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for some boards)
  }
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("=== Counter Library Example ===");
  Serial.println("Demonstrating cCount and cTime classes");
  Serial.println();
}

void loop() {
  // Example 1: Simple counter - count to 5 and execute action
  buttonCounter.count(5);
  if (buttonCounter.start()) {
    Serial.println("Counter started!");
  }
  if (buttonCounter.finish()) {
    Serial.print("Counter reached goal! Count: ");
    Serial.println(buttonCounter.get());
    Serial.println();
  }
  
  // Example 2: Counter with flags - LED blinker pattern
  ledBlinker.count(3, currentState, nextState);
  if (ledBlinker.finish()) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    Serial.println(digitalRead(LED_PIN) ? "LED ON" : "LED OFF");
  }
  
  // Example 3: Timer - execute every 1 second (1,000,000 microseconds)
  intervalTimer.wait(1000000);
  if (intervalTimer.start()) {
    Serial.println("Timer started!");
  }
  if (intervalTimer.finish()) {
    loopCount++;
    Serial.print("Timer event - Second: ");
    Serial.println(loopCount);
    Serial.print("Elapsed time (us): ");
    Serial.println(intervalTimer.get());
    
    // Use callback functionality
    ledBlinker.after(printMessage, "Callback executed!");
  }
  
  // Example 4: Reset demonstration - reset counter after 20 seconds
  if (loopCount >= 20) {
    Serial.println("\n=== Resetting all counters and timer ===\n");
    buttonCounter.reset();
    ledBlinker.reset();
    intervalTimer.reset();
    loopCount = 0;
  }
  
  // Small delay for stability
  delay(100);
}

/**
 * @brief Callback function example
 * @param message The message to print
 */
void printMessage(const char* message) {
  Serial.print("Callback: ");
  Serial.println(message);
}
