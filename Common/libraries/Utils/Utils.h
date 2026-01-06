#ifndef UTILS_H
#define UTILS_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(ESP_PLATFORM)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_err.h"// Include ESP error codes
#include "esp_log.h"// Add ESP logging support
#include "esp_timer.h"// Include ESP timer support
#else
// Add other platform includes as needed
#endif
#if __has_include("stdint.h")
#include <stdint.h>
#endif
#if __has_include(<utility>)
#include <utility>
#endif
#include <string>
#include <map>
/**
 * @brief Utility functions for development boards
 * 
 * This library provides common utility functions that can be used
 * across different development boards (Arduino, ESP32, etc.)
 */

namespace Utils {
  
  /**
   * @brief Map a value from one range to another
   * @param value The value to map
   * @param fromLow Lower bound of the value's current range
   * @param fromHigh Upper bound of the value's current range
   * @param toLow Lower bound of the target range
   * @param toHigh Upper bound of the target range
   * @return Mapped value
   */
  inline long mapValue(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    if (fromLow == fromHigh) return toLow;
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
  }
  
  /**
   * @brief Constrain a value within a range
   * @param value The value to constrain
   * @param min Minimum allowed value
   * @param max Maximum allowed value
   * @return Constrained value
   */
  template<typename T>
  inline T constrainValue(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }
  
  /**
   * @brief Calculate average of an array
   * @param values Array of values
   * @param size Size of the array
   * @return Average value
   */
  template<typename T>
  inline float average(T* values, int size) {
    if (values == nullptr || size == 0) return 0.0;
    
    float sum = 0;
    for (int i = 0; i < size; i++) {
      sum += values[i];
    }
    return sum / size;
  }
  template<typename T>
  inline bool inMap(const std::string& key, const std::map<std::string, T>& myMap){
      return myMap.find(key) != myMap.end();
  }
}
#endif // UTILS_H
