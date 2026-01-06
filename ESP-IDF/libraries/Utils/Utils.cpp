#include "Utils.h"

namespace Utils {

void taskManager::statusTask(BaseType_t gpTaskResult, const char* TAG, const char* taskName) {
   if(gpTaskResult != pdPASS) {
      ESP_LOGE(TAG, "Failed to create %s task!", taskName);
   } else {
      ESP_LOGI(TAG, "%s Task created successfully", taskName);
   }
}

void taskManager::add(const std::string& name, TaskFunction_t taskFunc, void* param, UBaseType_t priority, BaseType_t core, uint32_t stackSize){
    taskStruct newTask;
    newTask.name = name;
    newTask.param = param;
    newTask.priority = priority;
    newTask.core = core;
    newTask.stackSize = stackSize;
    BaseType_t result = xTaskCreatePinnedToCore(taskFunc, name.c_str(), stackSize, param, priority, &newTask.handle, core);
    statusTask(result, "TASK_MANAGER", name.c_str());
    if(result == pdPASS){
        _taskMap[name] = newTask;
        esp_err_t wdt_err = esp_task_wdt_add(newTask.handle);
        if(wdt_err != ESP_OK){
            ESP_LOGW("TASK_MANAGER", "Failed to add task %s to watchdog: %s", name.c_str(), esp_err_to_name(wdt_err));
        }
    }
}

void taskManager::del(const std::string& name){
    if(inMap(name, _taskMap)){
        esp_err_t wdt_err = esp_task_wdt_delete(_taskMap[name].handle);
        if(wdt_err != ESP_OK && wdt_err != ESP_ERR_NOT_FOUND){
            ESP_LOGW("TASK_MANAGER", "Failed to unsubscribe task %s from watchdog: %s", name.c_str(), esp_err_to_name(wdt_err));
        }
        vTaskDelete(_taskMap[name].handle);
        _taskMap.erase(name);
        ESP_LOGI("TASK_MANAGER", "Deleted task: %s", name.c_str());
    } else {
        ESP_LOGE("TASK_MANAGER", "Task not found: %s", name.c_str());
    }
}

void taskManager::resetWatchdog(const std::string& name){
    if(inMap(name, _taskMap)){
        esp_err_t wdt_err = esp_task_wdt_reset();
        if(wdt_err != ESP_OK){
            ESP_LOGW("TASK_MANAGER", "Failed to reset watchdog for task %s: %s", name.c_str(), esp_err_to_name(wdt_err));
        }
    } else {
        ESP_LOGE("TASK_MANAGER", "Task not found: %s", name.c_str());
    }
}
taskManager::~taskManager(){
    for(auto const& taskPair : _taskMap){
        esp_err_t wdt_err = esp_task_wdt_delete(taskPair.second.handle);
        if(wdt_err != ESP_OK && wdt_err != ESP_ERR_NOT_FOUND){
            ESP_LOGW("TASK_MANAGER", "Failed to unsubscribe task %s from watchdog: %s", taskPair.first.c_str(), esp_err_to_name(wdt_err));
        }
        vTaskDelete(taskPair.second.handle);
        ESP_LOGI("TASK_MANAGER", "Deleted task: %s", taskPair.first.c_str());
    }
    _taskMap.clear();
}

} // namespace Utils