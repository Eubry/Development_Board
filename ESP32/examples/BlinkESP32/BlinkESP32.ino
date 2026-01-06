/*
  ESP32 Blink
  
  Blinks the built-in LED on ESP32 development board.
  
  Hardware:
  - ESP32 development board
  - Most ESP32 boards have a built-in LED on GPIO2
  
  Note: The built-in LED pin may vary depending on your ESP32 board.
  Common pins: GPIO2, GPIO5, or GPIO25
*/

// Define LED pin - GPIO2 is common for many ESP32 boards
const int LED_PIN = 2;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("ESP32 Blink Example");
  
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Turn the LED on
  Serial.println("LED ON");
  delay(1000);                   // Wait for a second
  
  digitalWrite(LED_PIN, LOW);    // Turn the LED off
  Serial.println("LED OFF");
  delay(1000);                   // Wait for a second
}
