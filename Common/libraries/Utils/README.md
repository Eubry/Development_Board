# Utils Library

A common utility library for development boards (Arduino, ESP32, etc.)

## Description

This library provides common utility functions that can be used across different development board platforms. It includes functions for value mapping, constraining, and averaging.

## Features

- **mapValue()**: Map a value from one range to another
- **constrainValue()**: Constrain a value within a specified range (template-based for any numeric type)
- **average()**: Calculate the average of an array of values (template-based for any numeric type)

## Installation

### Arduino IDE
1. Copy the `Utils` folder to your Arduino libraries directory
   - Windows: `Documents\Arduino\libraries\`
   - Mac: `Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    Utils
```

## Usage

```cpp
#include <Utils.h>

void setup() {
  Serial.begin(9600);
  
  // Map a value from 0-1023 to 0-255
  int sensorValue = 512;
  int mapped = Utils::mapValue(sensorValue, 0, 1023, 0, 255);
  
  // Constrain a value (works with any numeric type)
  int constrained = Utils::constrainValue(300, 0, 255); // Returns 255
  float tempConstrained = Utils::constrainValue(102.5f, -40.0f, 85.0f); // Returns 85.0
  
  // Calculate average (works with any numeric type)
  int values[] = {10, 20, 30, 40, 50};
  float avg = Utils::average(values, 5); // Returns 30.0
  
  float temperatures[] = {21.5, 22.3, 23.1, 22.8};
  float avgTemp = Utils::average(temperatures, 4); // Returns 22.425
}

void loop() {
  // Your code here
}
```

## API Reference

### mapValue()
```cpp
long mapValue(long value, long fromLow, long fromHigh, long toLow, long toHigh)
```
Maps a value from one range to another range proportionally.

**Parameters:**
- `value` - The value to map
- `fromLow` - Lower bound of the input range
- `fromHigh` - Upper bound of the input range
- `toLow` - Lower bound of the output range
- `toHigh` - Upper bound of the output range

**Returns:** Mapped value as `long`

**Example:**
```cpp
// Map analog reading (0-1023) to voltage (0-5000 mV)
int voltage = Utils::mapValue(analogRead(A0), 0, 1023, 0, 5000);
```

### constrainValue()
```cpp
template<typename T>
T constrainValue(T value, T min, T max)
```
Constrains a value to be within a specified range. Template-based to work with any numeric type.

**Parameters:**
- `value` - The value to constrain
- `min` - Minimum allowed value
- `max` - Maximum allowed value

**Returns:** The constrained value (same type as input)

**Example:**
```cpp
int pwm = Utils::constrainValue(pwmValue, 0, 255);
float temp = Utils::constrainValue(sensor.read(), -40.0f, 125.0f);
```

### average()
```cpp
template<typename T>
float average(T* values, int size)
```
Calculates the arithmetic mean of an array. Template-based to work with any numeric type.

**Parameters:**
- `values` - Pointer to the array of values
- `size` - Number of elements in the array

**Returns:** Average as `float` (returns 0.0 if array is null or size is 0)

**Example:**
```cpp
int readings[10] = {100, 102, 98, 101, 99, 103, 97, 100, 102, 98};
float avg = Utils::average(readings, 10); // Returns 100.0
```

## License

Open source - free to use and modify.
