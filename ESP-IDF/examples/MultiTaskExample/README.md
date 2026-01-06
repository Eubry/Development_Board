# Multi-Task Manager Example

Advanced example demonstrating multiple concurrent FreeRTOS tasks with different priorities and core assignments.

## What This Example Does

- Creates 4 concurrent tasks with different priorities
- Demonstrates core affinity (pinning tasks to specific cores)
- Shows inter-task communication through shared variables
- Monitors system resources (heap memory, uptime)
- Implements different task patterns:
  - High-frequency sensor reading (50ms intervals)
  - Variable-rate LED control based on sensor data
  - Periodic system monitoring (5-second intervals)
  - Background computation task

## Hardware Required

- ESP32 development board (dual-core)
- Built-in LED on GPIO2
- Boot button on GPIO0 (optional)

## Task Architecture

| Task Name | Priority | Core | Stack | Purpose |
|-----------|----------|------|-------|---------|
| sensorTask | 10 (High) | 1 | 3072 | Read sensor at 50ms intervals |
| ledControl | 5 (Medium) | 0 | 2048 | Control LED based on sensor value |
| compute | 3 (Low-Med) | 0 | 3072 | Background computation |
| monitor | 2 (Low) | 1 | 4096 | Log system status every 5s |

## How to Use

### Build and Flash

```bash
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## Expected Output

```
I (xxx) MultiTaskExample: ╔════════════════════════════════════════╗
I (xxx) MultiTaskExample: ║  Multi-Task Manager Example           ║
I (xxx) MultiTaskExample: ║  ESP-IDF Version: v5.x                ║
I (xxx) MultiTaskExample: ╚════════════════════════════════════════╝

I (xxx) MultiTaskExample: → Creating sensor task (Priority: 10, Core: 1)
I (xxx) TASK_MANAGER: sensorTask Task created successfully
I (xxx) MultiTaskExample: → Creating LED control task (Priority: 5, Core: 0)
I (xxx) TASK_MANAGER: ledControl Task created successfully
...
I (xxx) SensorTask: Sensor task started on Core 1
I (xxx) LEDTask: LED control task started on Core 0
I (xxx) MonitorTask: Monitor task started on Core 1
I (xxx) ComputeTask: Compute task started on Core 0
...
I (xxx) MonitorTask: === System Status ===
I (xxx) MonitorTask: Uptime: 5 seconds
I (xxx) MonitorTask: Current sensor value: 45
I (xxx) MonitorTask: Free heap: 245672 bytes
I (xxx) MonitorTask: ====================
```

## Key Concepts Demonstrated

### 1. Task Priority

Higher priority tasks preempt lower priority tasks:
- **Priority 10**: Sensor reading (critical, time-sensitive)
- **Priority 5**: LED control (important, visible feedback)
- **Priority 3**: Computation (can be delayed)
- **Priority 2**: Monitoring (lowest priority, periodic)

### 2. Core Affinity

Tasks are pinned to specific cores:
- **Core 0 (PRO_CPU)**: LED control, computation
- **Core 1 (APP_CPU)**: Sensor reading, monitoring

This balances the workload across both CPU cores.

### 3. Inter-Task Communication

Shared variables allow tasks to communicate:
```cpp
volatile int sensorValue = 0;  // Written by sensorTask, read by others
```

### 4. Different Task Patterns

- **Periodic**: Fixed interval execution (monitor every 5s)
- **High-frequency**: Rapid polling (sensor every 50ms)
- **Reactive**: Behavior based on data (LED blink rate varies)

### 5. Stack Size Considerations

Different tasks need different stack sizes:
- 2048 bytes: Simple LED control
- 3072 bytes: Sensor and computation tasks
- 4096 bytes: Monitor task (logging uses more stack)

## Observing Task Behavior

### LED Behavior

- **Sensor value < 50**: LED blinks slowly (500ms on/off)
- **Sensor value > 50**: LED blinks fast (100ms on/off)

### System Monitoring

Every 5 seconds, the monitor task logs:
- System uptime
- Current sensor value
- Available heap memory
- Minimum heap (lowest point since boot)

## Troubleshooting

**Tasks not starting:**
- Check stack size (increase if needed)
- Verify available heap memory
- Check task creation logs

**Watchdog timeout:**
- Ensure `resetWatchdog()` is called regularly in each task
- Reduce workload in task loops
- Increase task execution frequency

**Unexpected behavior:**
- Monitor serial output for error messages
- Check task priorities (higher = more CPU time)
- Verify core assignments

## Experiments to Try

1. **Change priorities**: Make compute task higher priority than sensor
2. **Modify core assignment**: Move all tasks to Core 1
3. **Adjust intervals**: Make sensor task slower, monitor faster
4. **Add new tasks**: Create additional tasks with different behaviors
5. **Stress test**: Increase computation complexity and monitor heap usage

## Next Steps

- See TaskLifecycleExample for dynamic task creation/deletion
- Study ESP-IDF FreeRTOS documentation for advanced features
- Experiment with task synchronization primitives (semaphores, queues)
