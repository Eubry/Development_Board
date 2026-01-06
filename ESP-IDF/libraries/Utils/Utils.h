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
namespace Utils{
    template<typename T>
    inline bool inMap(const std::string& key, const std::map<std::string, T>& myMap){
        return myMap.find(key) != myMap.end();
    }
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