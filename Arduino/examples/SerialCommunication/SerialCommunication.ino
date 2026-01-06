/*
  Serial Communication
  
  Demonstrates how to use Serial communication on Arduino.
  Sends data to the computer and receives commands.
  
  Hardware:
  - Arduino board (Uno, Nano, Mega, etc.)
  - USB connection to computer
  
  Usage:
  1. Upload the sketch
  2. Open Serial Monitor (Tools > Serial Monitor)
  3. Set baud rate to 9600
  4. Type commands to send to Arduino
*/

const int LED_PIN = 13;

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  
  // Wait for serial port to connect (needed for native USB boards)
  while (!Serial) {
    ; 
  }
  
  Serial.println("Arduino Serial Communication Example");
  Serial.println("Commands:");
  Serial.println("  'on'  - Turn LED on");
  Serial.println("  'off' - Turn LED off");
  Serial.println("  'status' - Get LED status");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming string
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove whitespace
    
    // Process the command
    if (command == "on") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED turned ON");
    } 
    else if (command == "off") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED turned OFF");
    }
    else if (command == "status") {
      int state = digitalRead(LED_PIN);
      Serial.print("LED is ");
      Serial.println(state == HIGH ? "ON" : "OFF");
    }
    else {
      Serial.println("Unknown command: " + command);
    }
  }
}
