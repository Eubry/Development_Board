# ESP-IDF Code and Libraries

This directory contains ESP-IDF framework specific code and libraries for ESP32 development.

## 📁 Structure

- **examples/** - ESP-IDF example projects demonstrating library usage
  - **BasicTaskExample** - Simple single-task LED blink
  - **MultiTaskExample** - Multi-core task management with priorities
  - **TaskLifecycleExample** - Dynamic task creation and deletion
- **libraries/** - ESP-IDF specific libraries
  - **Utils** - FreeRTOS task manager and utilities
   - **drvMotor** - Dual DC motor driver abstraction (L293D/DRV8833)

## 🚀 Getting Started

### Prerequisites

**ESP-IDF Framework**
1. Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) (version 4.4 or later recommended)
2. Follow the official [ESP-IDF setup guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
3. Set up your environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh  # Linux/Mac
   # OR
   %userprofile%\esp\esp-idf\export.bat  # Windows
   ```

### Hardware

- ESP32 development board (ESP32-DevKitC, ESP32-WROOM, ESP32-S3, etc.)
- USB cable (usually micro-USB or USB-C depending on your board)

### Using Libraries

To use ESP-IDF libraries from this repository in your project:

1. Copy the library folder to your project's `components` directory:
   ```bash
   cp -r ESP-IDF/libraries/Utils my-project/components/
   ```

2. Include in your code:
   ```cpp
   #include "Utils.h"
   ```

3. Build your project:
   ```bash
   idf.py build
   ```

## 📚 Available Libraries

### Utils (ESP-IDF)
ESP-IDF specific utility library with FreeRTOS task management.

**Features:**
- **taskManager** - Advanced FreeRTOS task management class
  - Create and manage multiple tasks
  - Configurable task priority, core affinity, and stack size
  - Automatic watchdog timer integration
  - Task lifecycle management (add/delete)
  - Safe task deletion with cleanup
- **inMap()** - Template function to check if a key exists in a std::map

**Example:**
```cpp
#include "Utils.h"

Utils::taskManager tasks;

void myTask(void* param) {
    while(1) {
        // Task code here
        tasks.resetWatchdog("myTask");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main() {
    // Create a task on core 1 with priority 5
    tasks.add("myTask", myTask, NULL, 5, 1, 4096);
}
```

See [libraries/Utils/README.md](libraries/Utils/README.md) for detailed documentation.

### drvMotor (ESP-IDF)
ESP-IDF motor driver abstraction for two-wheel DC motor control.

**Features:**
- Supports L293D pin mapping (enable + direction pins)
- Supports DRV8833 pin mapping (dual input per motor)
- Unified `motMgr` API for left/right motor control

**Example:**
```cpp
#include "motorMgr.h"

extern "C" void app_main(void) {
   motMgr::pinL293D left = {.en = 18, .inA = 5, .inB = 17};
   motMgr::pinL293D right = {.en = 19, .inA = 16, .inB = 4};

   motMgr motors(left, right);
   motors.begin();
   motors.move(120, 120);
}
```

See [libraries/drvMotor/README.md](libraries/drvMotor/README.md) for detailed documentation.

## 📖 Available Examples

### BasicTaskExample
Simple introduction to the taskManager class.

**What it demonstrates:**
- Creating a single FreeRTOS task
- Watchdog timer integration
- Basic task loop with delays
- LED control

**Hardware Required:** ESP32 board with LED on GPIO2

[View Example →](examples/BasicTaskExample/)

### MultiTaskExample
Advanced multi-tasking with priorities and core affinity.

**What it demonstrates:**
- Creating 4 concurrent tasks
- Task priority management (2-10)
- Core affinity (pinning to Core 0 or Core 1)
- Inter-task communication via shared variables
- System resource monitoring

**Hardware Required:** ESP32 dual-core board with LED on GPIO2

[View Example →](examples/MultiTaskExample/)

### TaskLifecycleExample
Dynamic task creation and deletion patterns.

**What it demonstrates:**
- Runtime task creation (not just at startup)
- Safe task deletion with cleanup
- Different task lifetime patterns (short-lived, continuous, event-driven)
- Memory management and leak detection
- Task state management

**Hardware Required:** ESP32 board with LED on GPIO2

[View Example →](examples/TaskLifecycleExample/)

### Running the Examples

```bash
# Navigate to an example directory
cd ESP-IDF/examples/BasicTaskExample

# Build and flash
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor

# Exit monitor with: Ctrl+]
```

## 🌟 ESP-IDF vs Arduino Framework

**When to use ESP-IDF:**
- Need direct access to FreeRTOS APIs
- Require fine-grained control over ESP32 features
- Building production-ready firmware
- Need maximum performance and efficiency
- Working with advanced ESP32 features (WiFi mesh, BLE, etc.)

**When to use Arduino:**
- Rapid prototyping
- Learning embedded development
- Using Arduino-compatible shields/libraries
- Simple projects without real-time requirements

## 🔧 ESP-IDF Features

The ESP-IDF framework provides:
- **FreeRTOS**: Real-time operating system with multitasking
- **WiFi & Bluetooth**: Full networking stack
- **Low Power**: Deep sleep and power management
- **Peripherals**: Complete control over all ESP32 hardware
- **OTA Updates**: Over-the-air firmware updates
- **Security**: Encryption, secure boot, flash encryption

## 📝 Creating Projects

### Standard ESP-IDF Project Structure

```
my-esp-idf-project/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   └── main.c
├── components/
│   └── Utils/          # Copy libraries here
└── sdkconfig
```

### Quick Start Command

```bash
# Create new project
idf.py create-project my-project

# Add library as component
cp -r ESP-IDF/libraries/Utils my-project/components/

# Configure project
cd my-project
idf.py menuconfig

# Build
idf.py build

# Flash to device
idf.py -p /dev/ttyUSB0 flash monitor
```

## 🤝 Contributing

When contributing ESP-IDF libraries:
- Follow ESP-IDF coding standards
- Include comprehensive documentation
- Test on multiple ESP32 variants (ESP32, ESP32-S2, ESP32-S3, ESP32-C3)
- Use ESP-IDF's logging system (ESP_LOG*)
- Handle errors properly with esp_err_t
- Include component.mk or CMakeLists.txt for component integration

## 📖 Documentation

- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)

## ⚠️ Important Notes

- ESP-IDF libraries are **not compatible** with Arduino framework without modification
- Use the appropriate framework for your project requirements
- FreeRTOS tasks require proper stack size allocation to avoid stack overflow
- Always enable watchdog timer for production code
- Test thoroughly on your target ESP32 variant

## 📝 License

This project is open source. Please check individual files for specific license information.
