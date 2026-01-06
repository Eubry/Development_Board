/*
  ESP32 WiFi Scanner
  
  Scans for available WiFi networks and displays them via Serial Monitor.
  Demonstrates WiFi capabilities of ESP32.
  
  Hardware:
  - ESP32 development board
  
  Usage:
  1. Upload the sketch
  2. Open Serial Monitor (baud rate: 115200)
  3. Watch as ESP32 scans for nearby WiFi networks
*/

#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  
  // Wait for serial connection
  delay(1000);
  
  Serial.println("ESP32 WiFi Scanner");
  Serial.println("==================");
  
  // Set WiFi to station mode and disconnect from any AP
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  Serial.println("\nScanning for WiFi networks...");
  
  // WiFi.scanNetworks returns the number of networks found
  int numNetworks = WiFi.scanNetworks();
  
  if (numNetworks == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(numNetworks);
    Serial.println(" network(s) found:");
    Serial.println("-----------------------------------");
    
    for (int i = 0; i < numNetworks; i++) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm) ");
      Serial.print(WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Encrypted");
      Serial.println();
    }
  }
  
  Serial.println("-----------------------------------");
  
  // Wait 5 seconds before next scan
  delay(5000);
}
