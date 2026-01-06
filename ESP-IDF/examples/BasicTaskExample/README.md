# Basic Task Manager Example

A simple example demonstrating the ESP-IDF Utils taskManager class.

## What This Example Does

- Creates a single FreeRTOS task using taskManager
- Blinks an LED connected to GPIO2
- Demonstrates watchdog timer integration
- Shows basic task lifecycle management

## Hardware Required

- ESP32 development board
- Built-in LED on GPIO2 (or external LED with resistor)

## How to Use

### Configure the Project

```bash
idf.py menuconfig
```

### Build and Flash

```bash
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

Replace `/dev/ttyUSB0` with your actual serial port (e.g., `COM3` on Windows).

## Expected Output

```
I (xxx) BasicExample: === Basic Task Manager Example ===
I (xxx) BasicExample: ESP-IDF Version: v5.x
I (xxx) BasicExample: Creating LED blink task...
I (xxx) TASK_MANAGER: ledBlink Task created successfully
I (xxx) BasicExample: Task created. LED should be blinking on GPIO2
I (xxx) BasicExample: LED ON
I (xxx) BasicExample: LED OFF
I (xxx) BasicExample: LED ON
...
```

The LED should blink on and off every 500ms.

## Key Concepts

### Task Creation

```cpp
tasks.add("ledBlink", ledBlinkTask, NULL, 1, 0, 2048);
```

- **Name**: "ledBlink" - unique identifier
- **Function**: ledBlinkTask - task function pointer
- **Parameters**: NULL - no parameters passed
- **Priority**: 1 - low priority (sufficient for LED blink)
- **Core**: 0 - run on core 0 (PRO_CPU)
- **Stack Size**: 2048 bytes

### Watchdog Reset

```cpp
tasks.resetWatchdog("ledBlink");
```

Called periodically within the task loop to prevent watchdog timeout.

## Troubleshooting

- **LED not blinking**: Check if GPIO2 is the correct pin for your board
- **Task creation failed**: Increase stack size or check available memory
- **Watchdog timeout**: Call `resetWatchdog()` more frequently in task loop

## Next Steps

- Try changing the blink interval
- Modify the GPIO pin
- Experiment with different task priorities
- See the MultiTaskExample for more complex scenarios
