/**
 * @brief Utility functions for ESP32 development
 * 
 * This library provides common utility functions specifically for ESP32 ESP-IDF
 * development using ESP-IDF framework.
 */
#ifndef ESPUTILS_H
#define ESPUTILS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "esp_err.h"// Include ESP error codes
#include "esp_log.h"// Add ESP logging support
#include "esp_timer.h"// Include ESP timer support
#include <stdint.h>
#include <string>
#include <map>
#include <variant>
#include <any>

namespace Utils{
    template<typename T>
    inline bool inMap(const std::string& key, const std::map<std::string, T>& myMap){
        return myMap.find(key) != myMap.end();
    }
    //-----------------------------------------
    //-----------------------------------------
    //Task Manager Class
    /**
     * @class taskManager
     * @brief Manages FreeRTOS tasks by name, allowing creation, deletion, and watchdog reset.
     *
     * The taskManager class provides an interface to manage FreeRTOS tasks using a map of task names to task structures.
     * It allows adding new tasks, deleting existing ones by name, and resetting the watchdog for a specific task.
     *
     * Internal task information is stored in the taskStruct structure, which includes the task handle, name, parameters,
     * priority, core affinity, and stack size.
     *
     * Public Methods:
     * - taskManager(): Constructor.
     * - void add(const std::string& name, TaskFunction_t taskFunc, void* param = NULL, UBaseType_t priority = 1, BaseType_t core = 0, uint32_t stackSize = 1024): Adds a new task.
     * - void del(const std::string& name): Deletes a task by its name.
     * - void resetWatchdog(const std::string& name): Resets the watchdog timer for a task by name.
     * - ~taskManager(): Destructor.
     */
    class taskManager{
    private:
    struct taskStruct{
        TaskHandle_t handle;
        std::string name;
        void* param = NULL;
        UBaseType_t priority = 1;
        BaseType_t core = 0;
        uint32_t stackSize = 1024;
    };
    std::map <std::string, taskStruct> _taskMap;
    void statusTask(BaseType_t gpTaskResult, const char* TAG, const char* taskName);
    public:
        taskManager(){};
        void add(const std::string& name, TaskFunction_t taskFunc, void* param = NULL, UBaseType_t priority = 1, BaseType_t core = 0, uint32_t stackSize = 1024);
        //Delete task by name
        void del(const std::string& name);
        //Reset watchdog for a task by name
        void resetWatchdog(const std::string& name);
        ~taskManager();
  };
}
#endif // UTILS_H