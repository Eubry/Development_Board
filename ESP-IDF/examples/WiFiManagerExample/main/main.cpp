/**
 * @file main.cpp
 * @brief Example demonstrating the wifiManager library for WiFi connectivity
 * @version 1.0.0
 * @date 2026-01-06
 * @author Eubry Gomez Ramirez
 * 
 * This example demonstrates:
 * - Connecting to WiFi in Station (STA) mode
 * - Setting a custom hostname
 * - Automatic reconnection with retry logic
 * - mDNS hostname publishing
 * - Retrieving IP address and hostname
 * - Monitoring connection status
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifiManager.h"

// WiFi credentials - CHANGE THESE TO YOUR NETWORK
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define WIFI_HOSTNAME "esp32-demo"
#define MAX_RETRY 5

// Tag for logging
static const char *TAG = "WiFiExample";

/**
 * @brief Main application entry point
 */
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "=== WiFi Manager Example ===");
    ESP_LOGI(TAG, "ESP-IDF Version: %s", esp_get_idf_version());
    
    // Create WiFi connection instance
    // Parameters: SSID, Password, Max Retries, Hostname
    wifiConnection wifi(WIFI_SSID, WIFI_PASSWORD, MAX_RETRY, WIFI_HOSTNAME);
    
    ESP_LOGI(TAG, "Connecting to WiFi...");
    ESP_LOGI(TAG, "SSID: %s", WIFI_SSID);
    ESP_LOGI(TAG, "Hostname: %s", WIFI_HOSTNAME);
    
    // Start WiFi in Station mode
    esp_err_t result = wifi.begin(WIFI_MODE_STA);
    
    if (result == ESP_OK) {
        ESP_LOGI(TAG, "Successfully connected to WiFi!");
        
        // Get and display network information
        std::string ipAddress = wifi.getIp();
        std::string hostname = wifi.getHostname();
        
        ESP_LOGI(TAG, "IP Address: %s", ipAddress.c_str());
        ESP_LOGI(TAG, "Hostname: %s", hostname.c_str());
        ESP_LOGI(TAG, "You can access this device at: %s.local", hostname.c_str());
        
        // Check connection status
        if (wifi.isConnected()) {
            ESP_LOGI(TAG, "Connection status: Connected");
        } else {
            ESP_LOGW(TAG, "Connection status: Not connected (this shouldn't happen)");
        }
        
    } else {
        ESP_LOGE(TAG, "Failed to connect to WiFi!");
        ESP_LOGE(TAG, "Error code: 0x%x", result);
        ESP_LOGE(TAG, "Please check:");
        ESP_LOGE(TAG, "  1. WiFi credentials are correct");
        ESP_LOGE(TAG, "  2. Router is powered on and in range");
        ESP_LOGE(TAG, "  3. Router uses 2.4 GHz (ESP32 doesn't support 5 GHz)");
    }
    
    ESP_LOGI(TAG, "WiFi is connected. Device is operational.");
    ESP_LOGI(TAG, "Monitoring connection... (Ctrl+] to exit)");
    
    // Main loop - monitor connection status
    int counter = 0;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));  // Check every 10 seconds
        
        if (wifi.isConnected()) {
            counter++;
            ESP_LOGI(TAG, "Still connected - uptime: %d seconds", counter * 10);
            
            // Periodically show IP address
            if (counter % 6 == 0) {  // Every minute
                ESP_LOGI(TAG, "Current IP: %s", wifi.getIp().c_str());
            }
        } else {
            ESP_LOGW(TAG, "Connection lost! WiFi will attempt to reconnect...");
            counter = 0;
        }
    }
    
    // Note: In a real application, you might want to properly stop WiFi
    // when shutting down:
    // wifi.stop();
}
