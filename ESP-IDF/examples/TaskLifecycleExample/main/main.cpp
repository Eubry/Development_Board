/**
 * @file main.cpp
 * @brief Task lifecycle management example with dynamic task creation/deletion
 * @version 1.0.0
 * @date 2026-01-05
 * @author Eubry Gomez Ramirez
 * 
 * This example demonstrates:
 * - Dynamic task creation during runtime
 * - Safe task deletion with cleanup
 * - Using inMap() to check task existence
 * - Task state management
 * - Practical use case: On-demand task spawning
 */

#include <stdio.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "Utils.h"

// GPIO configuration
#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_0

// Logging tags
static const char *TAG = "TaskLifecycle";

// Task manager
Utils::taskManager tasks;

// Task control flags
volatile bool runningTask1 = false;
volatile bool runningTask2 = false;
volatile bool runningTask3 = false;

/**
 * @brief Worker task 1 - Short-lived task
 */
void workerTask1(void* param) {
    int *workerId = (int*)param;
    ESP_LOGI(TAG, "Worker Task %d started", *workerId);
    runningTask1 = true;
    
    // Do some work for 5 seconds then exit
    for (int i = 0; i < 50; i++) {
        ESP_LOGI(TAG, "Worker %d: Iteration %d/50", *workerId, i + 1);
        tasks.resetWatchdog("worker1");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    ESP_LOGI(TAG, "Worker Task %d completed its work", *workerId);
    runningTask1 = false;
    
    // Task will be deleted by management loop
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Worker task 2 - Medium duration task
 */
void workerTask2(void* param) {
    int *workerId = (int*)param;
    ESP_LOGI(TAG, "Worker Task %d started (LED blinker)", *workerId);
    runningTask2 = true;
    
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    // Blink LED for 10 seconds
    for (int i = 0; i < 20; i++) {
        gpio_set_level(LED_PIN, i % 2);
        ESP_LOGI(TAG, "Worker %d: LED %s", *workerId, (i % 2) ? "ON" : "OFF");
        tasks.resetWatchdog("worker2");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
    gpio_set_level(LED_PIN, 0);
    ESP_LOGI(TAG, "Worker Task %d completed its work", *workerId);
    runningTask2 = false;
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Worker task 3 - Continuous task
 */
void workerTask3(void* param) {
    int *workerId = (int*)param;
    ESP_LOGI(TAG, "Worker Task %d started (continuous monitor)", *workerId);
    runningTask3 = true;
    
    int counter = 0;
    while(runningTask3) {
        counter++;
        ESP_LOGI(TAG, "Worker %d: Monitoring... (count: %d)", *workerId, counter);
        tasks.resetWatchdog("worker3");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    ESP_LOGI(TAG, "Worker Task %d shutting down", *workerId);
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Task manager - Creates and destroys tasks dynamically
 */
void taskManagerTask(void* param) {
    ESP_LOGI(TAG, "Task Manager started");
    
    int workerId1 = 1;
    int workerId2 = 2;
    int workerId3 = 3;
    
    int cycle = 0;
    
    while(1) {
        cycle++;
        ESP_LOGI(TAG, "\n╔════════════════════════════════╗");
        ESP_LOGI(TAG, "║  Management Cycle %2d           ║", cycle);
        ESP_LOGI(TAG, "╚════════════════════════════════╝");
        
        vTaskDelay(pdMS_TO_TICKS(3000));
        
        // Cycle 1: Create worker1
        if (cycle == 1) {
            ESP_LOGI(TAG, "→ Creating Worker Task 1 (short-lived)");
            tasks.add("worker1", workerTask1, &workerId1, 5, 1, 3072);
        }
        
        // Cycle 3: Create worker2
        if (cycle == 3) {
            ESP_LOGI(TAG, "→ Creating Worker Task 2 (LED blinker)");
            tasks.add("worker2", workerTask2, &workerId2, 5, 0, 2048);
        }
        
        // Cycle 5: Create worker3
        if (cycle == 5) {
            ESP_LOGI(TAG, "→ Creating Worker Task 3 (continuous)");
            tasks.add("worker3", workerTask3, &workerId3, 3, 1, 3072);
        }
        
        // Cycle 7: Delete worker1 if finished
        if (cycle == 7 && !runningTask1) {
            ESP_LOGI(TAG, "→ Deleting Worker Task 1 (completed)");
            tasks.del("worker1");
        }
        
        // Cycle 9: Delete worker2 if finished
        if (cycle == 9 && !runningTask2) {
            ESP_LOGI(TAG, "→ Deleting Worker Task 2 (completed)");
            tasks.del("worker2");
        }
        
        // Cycle 11: Stop and delete worker3
        if (cycle == 11) {
            ESP_LOGI(TAG, "→ Stopping Worker Task 3");
            runningTask3 = false;
            vTaskDelay(pdMS_TO_TICKS(2000));
            ESP_LOGI(TAG, "→ Deleting Worker Task 3");
            tasks.del("worker3");
        }
        
        // Cycle 13: Create all tasks again
        if (cycle == 13) {
            ESP_LOGI(TAG, "→ Restarting all worker tasks");
            runningTask1 = false;
            runningTask2 = false;
            runningTask3 = true;
            
            tasks.add("worker1", workerTask1, &workerId1, 5, 1, 3072);
            vTaskDelay(pdMS_TO_TICKS(1000));
            tasks.add("worker2", workerTask2, &workerId2, 5, 0, 2048);
            vTaskDelay(pdMS_TO_TICKS(1000));
            tasks.add("worker3", workerTask3, &workerId3, 3, 1, 3072);
        }
        
        // Status report
        ESP_LOGI(TAG, "Status - T1:%s T2:%s T3:%s | Heap: %lu bytes",
                 runningTask1 ? "RUN" : "---",
                 runningTask2 ? "RUN" : "---",
                 runningTask3 ? "RUN" : "---",
                 esp_get_free_heap_size());
        
        tasks.resetWatchdog("taskManager");
    }
}

/**
 * @brief Main application entry point
 */
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "╔════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║  Task Lifecycle Management Example    ║");
    ESP_LOGI(TAG, "║  ESP-IDF Version: %-20s║", esp_get_idf_version());
    ESP_LOGI(TAG, "╚════════════════════════════════════════╝\n");
    
    ESP_LOGI(TAG, "This example demonstrates:");
    ESP_LOGI(TAG, "  • Dynamic task creation");
    ESP_LOGI(TAG, "  • Safe task deletion");
    ESP_LOGI(TAG, "  • Task lifecycle management");
    ESP_LOGI(TAG, "  • Multiple worker task patterns\n");
    
    // Configure GPIO
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    
    ESP_LOGI(TAG, "Creating Task Manager...");
    tasks.add("taskManager", taskManagerTask, NULL, 8, 0, 4096);
    
    ESP_LOGI(TAG, "System running. Watch the task lifecycle in action!\n");
    
    // Main loop
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(30000));
        ESP_LOGI(TAG, "Main: System healthy | Free heap: %lu bytes",
                 esp_get_free_heap_size());
    }
}
