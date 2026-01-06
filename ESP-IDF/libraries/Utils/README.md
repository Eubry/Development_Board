# Utils Library (ESP-IDF)

ESP-IDF specific utility library providing advanced FreeRTOS task management for ESP32 development.

## Description

This library provides ESP-IDF framework utilities that leverage FreeRTOS and ESP32-specific features. It includes a powerful task management system for creating, monitoring, and controlling FreeRTOS tasks.

## Features

### taskManager Class

A comprehensive FreeRTOS task management system with the following capabilities:

- **Task Creation**: Create tasks with custom parameters
- **Core Affinity**: Pin tasks to specific CPU cores (0 or 1)
- **Priority Control**: Set task priorities (0-24)
- **Stack Management**: Configure stack size per task
- **Watchdog Integration**: Automatic watchdog timer management
- **Task Lifecycle**: Safe task creation and deletion
- **Task Registry**: Named task tracking and lookup

### Helper Functions

- **inMap()**: Check if a key exists in a std::map

## Installation

### ESP-IDF Project

1. Copy the Utils folder to your project's components directory:
   ```bash
   cp -r ESP-IDF/libraries/Utils your-project/components/
   ```

2. The component will be automatically detected by ESP-IDF build system

3. Include in your code:
   ```cpp
   #include "Utils.h"
   ```

## Usage

### Basic Task Creation

```cpp
#include "Utils.h"

Utils::taskManager tasks;

void ledTask(void* param) {
    while(1) {
        // Toggle LED
        gpio_set_level(GPIO_NUM_2, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(GPIO_NUM_2, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        
        // Reset watchdog for this task
        tasks.resetWatchdog("ledTask");
    }
}

void app_main() {
    // Create task: name, function, params, priority, core, stack size
    tasks.add("ledTask", ledTask, NULL, 1, 0, 2048);
}
```

### Advanced Task Management

```cpp
#include "Utils.h"

Utils::taskManager tasks;

void highPriorityTask(void* param) {
    int* counter = (int*)param;
    
    while(1) {
        (*counter)++;
        ESP_LOGI("HPTask", "Count: %d", *counter);
        
        tasks.resetWatchdog("highPriorityTask");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void lowPriorityTask(void* param) {
    while(1) {
        ESP_LOGI("LPTask", "Running background task");
        
        tasks.resetWatchdog("lowPriorityTask");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main() {
    int counter = 0;
    
    // High priority task on core 1
    tasks.add("highPriorityTask", highPriorityTask, &counter, 10, 1, 4096);
    
    // Low priority task on core 0
    tasks.add("lowPriorityTask", lowPriorityTask, NULL, 1, 0, 2048);
    
    // Later, delete a task if needed
    vTaskDelay(pdMS_TO_TICKS(10000));
    tasks.del("lowPriorityTask");
}
```

### Using inMap() Helper

```cpp
#include "Utils.h"
#include <map>

void checkConfig() {
    std::map<std::string, int> config = {
        {"maxConnections", 10},
        {"timeout", 5000},
        {"retries", 3}
    };
    
    if (Utils::inMap("timeout", config)) {
        ESP_LOGI("Config", "Timeout: %d", config["timeout"]);
    }
}
```

## API Reference

### taskManager::add()

```cpp
void add(const std::string& name, 
         TaskFunction_t taskFunc, 
         void* param = NULL, 
         UBaseType_t priority = 1, 
         BaseType_t core = 0, 
         uint32_t stackSize = 1024)
```

Creates and starts a new FreeRTOS task with watchdog integration.

**Parameters:**
- `name` - Unique task identifier
- `taskFunc` - Task function pointer (must have signature `void taskFunc(void*)`)
- `param` - Optional parameter to pass to the task
- `priority` - Task priority (0-24, higher = more priority)
- `core` - CPU core to run on (0 or 1, or `tskNO_AFFINITY` for any core)
- `stackSize` - Stack size in bytes (minimum 1024, typical 2048-4096)

**Example:**
```cpp
tasks.add("myTask", myTaskFunction, NULL, 5, 1, 4096);
```

### taskManager::del()

```cpp
void del(const std::string& name)
```

Safely deletes a task by name, including watchdog cleanup.

**Parameters:**
- `name` - Name of the task to delete

**Example:**
```cpp
tasks.del("myTask");
```

### taskManager::resetWatchdog()

```cpp
void resetWatchdog(const std::string& name)
```

Resets the watchdog timer for a specific task. Should be called periodically within the task's main loop.

**Parameters:**
- `name` - Name of the task to reset watchdog for

**Example:**
```cpp
void myTask(void* param) {
    while(1) {
        // Do work...
        tasks.resetWatchdog("myTask");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

### Utils::inMap()

```cpp
template<typename T>
bool inMap(const std::string& key, const std::map<std::string, T>& myMap)
```

Checks if a key exists in a std::map.

**Parameters:**
- `key` - The key to search for
- `myMap` - The map to search in

**Returns:** `true` if key exists, `false` otherwise

**Example:**
```cpp
std::map<std::string, int> data = {{"sensor1", 100}};
if (Utils::inMap("sensor1", data)) {
    // Key exists
}
```

## Task Priority Guidelines

| Priority | Use Case |
|----------|----------|
| 0-5 | Background tasks, logging, low-priority operations |
| 6-10 | Normal application tasks |
| 11-15 | Time-sensitive tasks, sensor reading |
| 16-20 | High-priority tasks, real-time control |
| 21-24 | Critical system tasks (use sparingly) |

## Stack Size Guidelines

| Task Type | Recommended Stack Size |
|-----------|----------------------|
| Simple LED blink | 1024 - 2048 bytes |
| Basic sensor reading | 2048 - 3072 bytes |
| WiFi/Network operations | 4096 - 8192 bytes |
| Complex processing | 8192+ bytes |

**Note:** Always monitor stack usage with `uxTaskGetStackHighWaterMark()` during development.

## Core Affinity

ESP32 has two cores (PRO_CPU and APP_CPU):
- **Core 0 (PRO_CPU)**: Usually runs WiFi/BT stack
- **Core 1 (APP_CPU)**: Available for user tasks

**Best Practices:**
- Pin computationally intensive tasks to Core 1
- Pin WiFi-related tasks to Core 0
- Use `tskNO_AFFINITY` for tasks that can run on either core

## Error Handling

The library uses ESP-IDF logging to report:
- Task creation success/failure
- Watchdog registration status
- Task deletion confirmation

Monitor serial output with:
```bash
idf.py monitor
```

## Requirements

- ESP-IDF 4.4 or later
- ESP32, ESP32-S2, ESP32-S3, or ESP32-C3
- FreeRTOS (included with ESP-IDF)

## Troubleshooting

**Task Won't Start:**
- Check stack size (increase if needed)
- Verify priority is valid (0-24)
- Ensure core is 0, 1, or tskNO_AFFINITY

**Watchdog Timeout:**
- Call `resetWatchdog()` more frequently in task loop
- Reduce processing time in task iterations
- Increase watchdog timeout in menuconfig

**Stack Overflow:**
- Increase stack size in `add()` call
- Reduce local variable usage in task
- Use dynamic allocation sparingly

## License

Open source - free to use and modify.
