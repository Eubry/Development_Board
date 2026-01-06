/**
 * @file CounterExample.ino
 * @brief Example demonstrating the Counter library on ESP32 with high-precision timing
 * @version 1.0.0
 * @date 2026-01-05
 * @author Eubry Gomez Ramirez
 * 
 * This example demonstrates:
 * - Using cCount for event counting
 * - Using cTime with ESP32's high-precision microsecond timer (esp_timer)
 * - Flag management with counters
 * - Callback functionality
 * - Serial output for monitoring
 */

#include "counter.h"

// Create counter and timer instances
cCount eventCounter;
cCount taskCounter;
cTime precisionTimer;
cTime blinkTimer;

// Flags for state management
bool currentFlag = false;
bool nextFlag = false;

// LED pin (most ESP32 boards use GPIO2)
const int LED_PIN = 2;

// Variables for demonstration
int eventCount = 0;
unsigned long startMillis = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000); // Give time for serial monitor to open
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("\n=== ESP32 Counter Library Example ===");
  Serial.println("Demonstrating high-precision cTime with esp_timer");
  Serial.println("Board: ESP32");
  Serial.println();
  
  startMillis = millis();
}

void loop() {
  // Example 1: High-precision timer - 500ms (500,000 microseconds)
  precisionTimer.wait(500000);
  if (precisionTimer.start()) {
    Serial.println("Precision timer started!");
  }
  if (precisionTimer.finish()) {
    eventCount++;
    Serial.print("Precision event #");
    Serial.print(eventCount);
    Serial.print(" | Elapsed: ");
    Serial.print(precisionTimer.get());
    Serial.println(" μs");
    
    // Display system uptime
    Serial.print("System uptime: ");
    Serial.print((millis() - startMillis) / 1000.0, 2);
    Serial.println(" seconds\n");
  }
  
  // Example 2: Event counter with flags - count to 10
  taskCounter.count(10, currentFlag, nextFlag);
  if (taskCounter.finish()) {
    Serial.println("Task counter reached 10!");
    Serial.print("Current count: ");
    Serial.println(taskCounter.get());
    
    // Use callback to execute function
    taskCounter.after(executeTask, eventCount);
  }
  
  // Example 3: LED blink with precise timing - 250ms (250,000 microseconds)
  blinkTimer.wait(250000);
  if (blinkTimer.finish()) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    if (digitalRead(LED_PIN)) {
      Serial.println("LED: ON");
    } else {
      Serial.println("LED: OFF");
    }
  }
  
  // Example 4: Simple counter - count loop iterations
  eventCounter.count(100);
  if (eventCounter.finish()) {
    Serial.println("Event counter: 100 iterations completed");
    Serial.print("Flags - Current: ");
    Serial.print(taskCounter.currentFlag());
    Serial.print(", Next: ");
    Serial.println(taskCounter.nextFlag());
    Serial.println();
  }
  
  // Example 5: Reset demonstration after 30 seconds
  if (eventCount >= 60) {  // 60 events * 500ms = 30 seconds
    Serial.println("\n=== Resetting all counters and timers ===");
    Serial.println("Demonstrating reset functionality...\n");
    
    eventCounter.reset();
    taskCounter.reset();
    precisionTimer.reset();
    blinkTimer.reset();
    eventCount = 0;
    currentFlag = false;
    nextFlag = false;
    
    delay(2000); // Pause briefly
  }
  
  // No delay needed - precise timing handled by cTime
}

/**
 * @brief Callback function executed when task counter finishes
 * @param count The current event count to display
 */
void executeTask(int count) {
  Serial.println("╔════════════════════════════════╗");
  Serial.println("║  CALLBACK FUNCTION EXECUTED!  ║");
  Serial.print("║  Total Events: ");
  Serial.print(count);
  if (count < 10) Serial.print(" ");
  Serial.println("              ║");
  Serial.println("╚════════════════════════════════╝");
  Serial.println();
}
