# Utils Library (ESP-IDF)

Utility helpers for ESP-IDF projects, centered on named FreeRTOS task lifecycle management.

## Overview

This component provides:

- `Utils::taskManager`: create, track, and delete tasks by name.
- `Utils::inMap(...)`: generic helper to check key existence in `std::map`.

The implementation is in `Utils.cpp` and the public API is in `Utils.h`.

## Dependencies

From `CMakeLists.txt`:

- `freertos`
- `esp_system`
- `esp_timer`

## Public API

### Namespace helper

```cpp
template<typename T>
bool Utils::inMap(const std::string& key, const std::map<std::string, T>& myMap);
```

Returns `true` if `key` exists in `myMap`.

### Class `Utils::taskManager`

```cpp
void add(const std::string& name,
         TaskFunction_t taskFunc,
         void* param = NULL,
         UBaseType_t priority = 1,
         BaseType_t core = 0,
         uint32_t stackSize = 1024);

void del(const std::string& name);
void resetWatchdog(const std::string& name);
```

Behavior summary:

- `add(...)`: creates a task with `xTaskCreatePinnedToCore` and stores its handle by name.
- `del(...)`: unsubscribes task from WDT with `esp_task_wdt_delete` (best effort), deletes it with `vTaskDelete`, then removes it from the internal map.
- `resetWatchdog(...)`: calls `esp_task_wdt_reset()` if task name exists in map.
- Destructor: iterates tracked tasks and deletes them.

## Usage

```cpp
#include "Utils.h"

Utils::taskManager tasks;

void blinkTask(void* pv) {
    while (true) {
        // Your task work here.
        tasks.resetWatchdog("blink");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

extern "C" void app_main(void) {
    tasks.add("blink", blinkTask, NULL, 3, 0, 2048);
}
```

## Notes And Current Limitations

- `taskManager` does not call `esp_task_wdt_add()` when creating tasks.
- `resetWatchdog(...)` calls `esp_task_wdt_reset()` for the current running task; the `name` parameter is only used for existence checking.
- Avoid deleting the currently running task from itself through `taskManager::del(...)` unless you intentionally design for that lifecycle.

## Include

```cpp
#include "Utils.h"
```
- Unsubscribes task from watchdog timer (`esp_task_wdt_delete()`)
- Deletes the FreeRTOS task (`vTaskDelete()`)
- Removes task from internal tracking map
- Logs deletion status or error if task not found

**Example:**
```cpp
tasks.del("myTask");
```

### taskManager::resetWatchdog()

```cpp
void resetWatchdog(const std::string& name)
```

Resets the watchdog timer for a specific task. Should be called periodically within the task's main loop to prevent watchdog timeout.

**Parameters:**
- `name` - Name of the task to reset watchdog for

**Behavior:**
- Verifies task exists in internal map
- Calls `esp_task_wdt_reset()` to reset the global watchdog timer
- Logs warning if reset fails
- Logs error if task not found

**Important:** This resets the global watchdog, so call it from within your task loop at regular intervals.

**Example:**
```cpp
void myTask(void* param) {
    while(1) {
inline bool inMap(const std::string& key, const std::map<std::string, T>& myMap)
```

Template helper function that checks if a key exists in a std::map.

**Parameters:**
- `key` - The key to search for
- `myMap` - The map to search in (const reference)

**Returns:** `true` if key exists, `false` otherwise

**Template Parameter:**
- `T` - The value type of the map (automatically deduced)

**Implementation:** Uses `find()` and compares against `end()` iterator

**Example:**
```cpp
std::map<std::string, int> data = {{"sensor1", 100}};
if (Utils::inMap("sensor1", data)) {
    // Key exists - safe to access data["sensor1"]
}

std::map<std::string, std::string> config = {{"wifi", "enabled"}};
if (Utils::inMap("mqtt", config)) {
    // This will be false - mqtt not in config
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

The library uses ESP-IDF logging (ESP_LOG) to report status and errors:
- **ESP_LOGI**: Task creation success, task deletion confirmation
- **ESP_LOGE**: Task creation failure, task not found errors
- **ESP_LOGW**: Watchdog operation warnings (subscription/unsubscription failures)

All logs are tagged with "TASK_MANAGER" for easy filtering.

Monitor serial output with:
```bash
idf.py monitor
```

## Internal Implementation Details

### taskStruct

Internal structure used to track task information:
```cpp
struct taskStruct {
    TaskHandle_t handle;        // FreeRTOS task handle
    std::string name;           // Task name
    void* param;                // Task parameter (default: NULL)
    UBaseType_t priority;       // Task priority (default: 1)
    BaseType_t core;            // CPU core affinity (default: 0)
    uint32_t stackSize;         // Stack size in bytes (default: 1024)
};
```

### Private Members

- `_taskMap`: `std::map<std::string, taskStruct>` - Stores all managed tasks indexed by name
- `statusTask()`: Private method that logs task creation success/failure

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
