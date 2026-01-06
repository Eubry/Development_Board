# Utils Library

A common utility library for development boards (Arduino, ESP32, etc.)

## Description

This library provides common utility functions that can be used across different development board platforms. It includes functions for value mapping, constraining, averaging, and timing.

## Features

- **mapValue()**: Map a value from one range to another
- **constrainValue()**: Constrain a value within a specified range
- **average()**: Calculate the average of an array of values
- **delayMs()**: Simple delay function with millisecond precision
- **getUptime()**: Get the current uptime since board started

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
  
  // Constrain a value
  int constrained = Utils::constrainValue(300, 0, 255); // Returns 255
  
  // Calculate average
  int values[] = {10, 20, 30, 40, 50};
  float avg = Utils::average(values, 5); // Returns 30.0
  
  // Get uptime
  unsigned long uptime = Utils::getUptime();
  Serial.print("Uptime: ");
  Serial.println(uptime);
}

void loop() {
  // Your code here
}
```

## License

Open source - free to use and modify.
