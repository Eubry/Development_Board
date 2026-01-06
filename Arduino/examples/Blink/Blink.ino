/*
  Blink
  
  Turns an LED on for one second, then off for one second, repeatedly.
  This is a basic example demonstrating digital output on Arduino.
  
  Hardware:
  - Arduino board (Uno, Nano, Mega, etc.)
  - Built-in LED on pin 13 (or external LED with resistor)
  
  Circuit:
  - Most Arduino boards have an LED connected to pin 13
  - If using external LED: Connect LED anode (+) to pin 13 through 220Ω resistor,
    cathode (-) to GND
*/

// Define LED pin - most Arduino boards have built-in LED on pin 13
const int LED_PIN = 13;

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Turn the LED on (HIGH voltage level)
  delay(1000);                   // Wait for a second
  digitalWrite(LED_PIN, LOW);    // Turn the LED off (LOW voltage level)
  delay(1000);                   // Wait for a second
}
