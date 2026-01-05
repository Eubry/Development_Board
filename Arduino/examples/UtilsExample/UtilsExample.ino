/*
  Utils Library Example
  
  Demonstrates the use of the Utils library functions.
  This example works on both Arduino and ESP32 boards.
  
  Hardware:
  - Arduino board (any type)
  
  Instructions:
  1. Copy the Utils library to your Arduino libraries folder
  2. Upload this sketch
  3. Open Serial Monitor (9600 baud)
*/

#include <Utils.h>

void setup() {
  Serial.begin(9600);
  
  // Wait for serial connection
  while (!Serial) {
    ;
  }
  
  Serial.println("Utils Library Example");
  Serial.println("====================\n");
  
  // Example 1: Map a value
  Serial.println("1. Map Value Example:");
  int sensorValue = 512;
  int mappedValue = Utils::mapValue(sensorValue, 0, 1023, 0, 255);
  Serial.print("   Sensor value (0-1023): ");
  Serial.println(sensorValue);
  Serial.print("   Mapped to (0-255): ");
  Serial.println(mappedValue);
  Serial.println();
  
  // Example 2: Constrain a value
  Serial.println("2. Constrain Value Example:");
  int testValue = 300;
  int constrained = Utils::constrainValue(testValue, 0, 255);
  Serial.print("   Original value: ");
  Serial.println(testValue);
  Serial.print("   Constrained (0-255): ");
  Serial.println(constrained);
  Serial.println();
  
  // Example 3: Calculate average
  Serial.println("3. Average Example:");
  int readings[] = {10, 20, 30, 40, 50};
  float avg = Utils::average(readings, 5);
  Serial.print("   Values: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(readings[i]);
    if (i < 4) Serial.print(", ");
  }
  Serial.println();
  Serial.print("   Average: ");
  Serial.println(avg);
  Serial.println();
  
  // Example 4: Get uptime
  Serial.println("4. Uptime Example:");
  unsigned long uptime = Utils::getUptime();
  Serial.print("   Uptime: ");
  Serial.print(uptime);
  Serial.println(" ms");
  Serial.println();
  
  Serial.println("All examples completed!");
}

void loop() {
  // Display uptime every 2 seconds
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate >= 2000) {
    lastUpdate = millis();
    Serial.print("Current uptime: ");
    Serial.print(Utils::getUptime());
    Serial.println(" ms");
  }
}
