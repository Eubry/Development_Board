/**
 * @file main.cpp
 * @brief Multi-task example demonstrating advanced taskManager features
 * @version 1.0.0
 * @date 2026-01-05
 * @author Eubry Gomez Ramirez
 * 
 * This example demonstrates:
 * - Creating multiple FreeRTOS tasks with different priorities
 * - Pinning tasks to specific CPU cores
 * - Inter-task communication using shared data
 * - Different task patterns (periodic, event-driven)
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "Utils.h"

// GPIO pins
#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_0  // Boot button on most ESP32 boards

// Tags for logging
static const char *TAG_MAIN = "MultiTaskExample";
static const char *TAG_SENSOR = "SensorTask";
static const char *TAG_LED = "LEDTask";
static const char *TAG_MONITOR = "MonitorTask";

// Shared data (protected by FreeRTOS scheduler)
volatile int sensorValue = 0;
volatile int taskCounter = 0;

// Task manager instance
Utils::taskManager tasks;

/**
 * @brief High-priority sensor reading task on Core 1
 * Simulates reading a sensor at high frequency
 */
void sensorTask(void* param) {
    ESP_LOGI(TAG_SENSOR, "Sensor task started on Core %d", xPortGetCoreID());
    
    int reading = 0;
    while(1) {
        // Simulate sensor reading (random value 0-100)
        reading = (reading + 1) % 101;
        sensorValue = reading;
        
        if (reading % 20 == 0) {
            ESP_LOGI(TAG_SENSOR, "Sensor reading: %d", sensorValue);
        }
        
        tasks.resetWatchdog("sensorTask");
        vTaskDelay(pdMS_TO_TICKS(50));  // Read every 50ms
    }
}

/**
 * @brief Medium-priority LED control task on Core 0
 * Controls LED based on sensor values
 */
void ledControlTask(void* param) {
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG_LED, "LED control task started on Core %d", xPortGetCoreID());
    
    while(1) {
        // LED blink rate based on sensor value
        if (sensorValue > 50) {
            // Fast blink for high values
            gpio_set_level(LED_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
        } else {
            // Slow blink for low values
            gpio_set_level(LED_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            gpio_set_level(LED_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        
        tasks.resetWatchdog("ledControl");
    }
}

/**
 * @brief Low-priority monitoring task on Core 1
 * Logs system status periodically
 */
void monitorTask(void* param) {
    ESP_LOGI(TAG_MONITOR, "Monitor task started on Core %d", xPortGetCoreID());
    
    while(1) {
        taskCounter++;
        
        // Log system status every 5 seconds
        ESP_LOGI(TAG_MONITOR, "=== System Status ===");
        ESP_LOGI(TAG_MONITOR, "Uptime: %lu seconds", taskCounter * 5);
        ESP_LOGI(TAG_MONITOR, "Current sensor value: %d", sensorValue);
        ESP_LOGI(TAG_MONITOR, "Free heap: %lu bytes", esp_get_free_heap_size());
        ESP_LOGI(TAG_MONITOR, "Min free heap: %lu bytes", esp_get_minimum_free_heap_size());
        ESP_LOGI(TAG_MONITOR, "====================\n");
        
        tasks.resetWatchdog("monitor");
        vTaskDelay(pdMS_TO_TICKS(5000));  // Monitor every 5 seconds
    }
}

/**
 * @brief Background computation task
 * Demonstrates computation-intensive task
 */
void computeTask(void* param) {
    ESP_LOGI("ComputeTask", "Compute task started on Core %d", xPortGetCoreID());
    
    uint32_t computeResult = 0;
    
    while(1) {
        // Simulate some computation
        for (int i = 0; i < 1000; i++) {
            computeResult += i * sensorValue;
        }
        
        // Log result occasionally
        static int logCount = 0;
        if (++logCount >= 100) {
            ESP_LOGI("ComputeTask", "Computation result: %lu", computeResult);
            logCount = 0;
        }
        
        tasks.resetWatchdog("compute");
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * @brief Main application entry point
 */
extern "C" void app_main(void)
{
    ESP_LOGI(TAG_MAIN, "╔════════════════════════════════════════╗");
    ESP_LOGI(TAG_MAIN, "║  Multi-Task Manager Example           ║");
    ESP_LOGI(TAG_MAIN, "║  ESP-IDF Version: %-20s║", esp_get_idf_version());
    ESP_LOGI(TAG_MAIN, "╚════════════════════════════════════════╝\n");
    
    // Configure GPIO
    ESP_LOGI(TAG_MAIN, "Configuring GPIO...");
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    
    // Create tasks with different priorities and core assignments
    ESP_LOGI(TAG_MAIN, "\nCreating tasks...\n");
    
    // High-priority sensor task on Core 1
    ESP_LOGI(TAG_MAIN, "→ Creating sensor task (Priority: 10, Core: 1)");
    tasks.add("sensorTask", sensorTask, NULL, 10, 1, 3072);
    
    // Medium-priority LED task on Core 0
    ESP_LOGI(TAG_MAIN, "→ Creating LED control task (Priority: 5, Core: 0)");
    tasks.add("ledControl", ledControlTask, NULL, 5, 0, 2048);
    
    // Low-priority monitor task on Core 1
    ESP_LOGI(TAG_MAIN, "→ Creating monitor task (Priority: 2, Core: 1)");
    tasks.add("monitor", monitorTask, NULL, 2, 1, 4096);
    
    // Background compute task on Core 0
    ESP_LOGI(TAG_MAIN, "→ Creating compute task (Priority: 3, Core: 0)");
    tasks.add("compute", computeTask, NULL, 3, 0, 3072);
    
    ESP_LOGI(TAG_MAIN, "\n✓ All tasks created successfully!");
    ESP_LOGI(TAG_MAIN, "System is now running with 4 concurrent tasks\n");
    
    // Main loop
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(30000));
        ESP_LOGI(TAG_MAIN, "Main loop: System healthy");
    }
}
