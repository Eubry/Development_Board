/**
 * @file main.cpp
 * @brief Basic example demonstrating the Utils taskManager class
 * @version 1.0.0
 * @date 2026-01-05
 * @author Eubry Gomez Ramirez
 * 
 * This example demonstrates:
 * - Creating a simple FreeRTOS task using taskManager
 * - Using watchdog timer integration
 * - Basic task loop with delays
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "Utils.h"

// GPIO pin for LED (most ESP32 boards use GPIO2)
#define LED_PIN GPIO_NUM_2

// Tag for logging
static const char *TAG = "BasicExample";

// Create task manager instance
Utils::taskManager tasks;

/**
 * @brief Simple LED blink task
 * @param param Task parameters (unused)
 */
void ledBlinkTask(void* param) {
    // Configure LED GPIO
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG, "LED Blink Task started");
    
    while(1) {
        // Turn LED ON
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "LED ON");
        vTaskDelay(pdMS_TO_TICKS(500));
        
        // Turn LED OFF
        gpio_set_level(LED_PIN, 0);
        ESP_LOGI(TAG, "LED OFF");
        vTaskDelay(pdMS_TO_TICKS(500));
        
        // Reset watchdog timer for this task
        tasks.resetWatchdog("ledBlink");
    }
}

/**
 * @brief Main application entry point
 */
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "=== Basic Task Manager Example ===");
    ESP_LOGI(TAG, "ESP-IDF Version: %s", esp_get_idf_version());
    
    // Create LED blink task
    // Parameters: name, function, params, priority, core, stack size
    ESP_LOGI(TAG, "Creating LED blink task...");
    tasks.add("ledBlink", ledBlinkTask, NULL, 1, 0, 2048);
    
    ESP_LOGI(TAG, "Task created. LED should be blinking on GPIO%d", LED_PIN);
    ESP_LOGI(TAG, "Main task will now idle...");
    
    // Main loop - in real applications, this could do other work
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
        ESP_LOGI(TAG, "Main task still running...");
    }
}
