# Task Lifecycle Management Example

Advanced example demonstrating dynamic task creation, deletion, and lifecycle management using the taskManager class.

## What This Example Does

This example creates a sophisticated task management system that:
- Dynamically creates tasks during runtime (not just at startup)
- Safely deletes tasks after they complete their work
- Demonstrates different task lifetime patterns:
  - **Short-lived**: Task runs for a fixed duration then exits
  - **Medium-duration**: Task performs specific job then exits
  - **Continuous**: Task runs until explicitly stopped
- Shows proper task cleanup and resource management
- Monitors system resources throughout task lifecycle

## Task Timeline

```
Cycle 1  → Create Worker 1 (short-lived, 5s duration)
Cycle 3  → Create Worker 2 (LED blinker, 10s duration)
Cycle 5  → Create Worker 3 (continuous monitor)
Cycle 7  → Delete Worker 1 (completed)
Cycle 9  → Delete Worker 2 (completed)
Cycle 11 → Stop & Delete Worker 3
Cycle 13 → Restart all workers (loop repeats)
```

Each cycle is ~3 seconds.

## Hardware Required

- ESP32 development board (dual-core)
- Built-in LED on GPIO2
- Boot button on GPIO0 (optional)

## How to Use

### Build and Flash

```bash
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## Expected Output

```
I (xxx) TaskLifecycle: ╔════════════════════════════════════════╗
I (xxx) TaskLifecycle: ║  Task Lifecycle Management Example    ║
I (xxx) TaskLifecycle: ╚════════════════════════════════════════╝

I (xxx) TaskLifecycle: ╔════════════════════════════════╗
I (xxx) TaskLifecycle: ║  Management Cycle  1           ║
I (xxx) TaskLifecycle: ╚════════════════════════════════╝
I (xxx) TaskLifecycle: → Creating Worker Task 1 (short-lived)
I (xxx) TASK_MANAGER: worker1 Task created successfully
I (xxx) TaskLifecycle: Worker Task 1 started
I (xxx) TaskLifecycle: Worker 1: Iteration 1/50
...
I (xxx) TaskLifecycle: Worker Task 1 completed its work
I (xxx) TaskLifecycle: → Deleting Worker Task 1 (completed)
I (xxx) TASK_MANAGER: Deleted task: worker1
...
```

## Key Concepts Demonstrated

### 1. Dynamic Task Creation

Tasks can be created anytime, not just during initialization:

```cpp
// Create task in cycle 1
if (cycle == 1) {
    tasks.add("worker1", workerTask1, &workerId1, 5, 1, 3072);
}
```

### 2. Graceful Task Termination

Two patterns for stopping tasks:

**Pattern A: Self-terminating (time-based)**
```cpp
void workerTask1(void* param) {
    // Do work for fixed duration
    for (int i = 0; i < 50; i++) {
        // ... work ...
    }
    // Signal completion
    runningTask1 = false;
    // Wait to be deleted
    while(1) { vTaskDelay(...); }
}
```

**Pattern B: External control (flag-based)**
```cpp
void workerTask3(void* param) {
    while(runningTask3) {  // Controlled by external flag
        // ... work ...
    }
    // Clean shutdown
}
```

### 3. Safe Task Deletion

Always ensure task is ready to be deleted:

```cpp
// Stop the task first
runningTask3 = false;
vTaskDelay(pdMS_TO_TICKS(2000));  // Give time to exit loop
// Now safe to delete
tasks.del("worker3");
```

### 4. Resource Monitoring

Track system health during task lifecycle:

```cpp
ESP_LOGI(TAG, "Free heap: %lu bytes", esp_get_free_heap_size());
```

Watch for memory leaks - heap should return to baseline after task deletion.

## Task Patterns Explained

### Short-Lived Task (Worker 1)

**Use Case**: One-time jobs, initialization, calibration

**Characteristics:**
- Fixed iteration count (50 iterations)
- Self-terminates when done
- Deleted by manager after completion

**Example Applications:**
- Sensor calibration routine
- One-time data processing
- Initialization sequence

### Medium-Duration Task (Worker 2)

**Use Case**: Specific jobs with clear end condition

**Characteristics:**
- Performs specific task (LED blink 20 times)
- Cleans up resources (turns LED off)
- Self-terminates when job done

**Example Applications:**
- Visual/audio feedback sequences
- File transfers
- Temporary monitoring

### Continuous Task (Worker 3)

**Use Case**: Long-running services, monitoring

**Characteristics:**
- Runs until explicitly stopped
- Controlled by external flag
- Graceful shutdown when flag cleared

**Example Applications:**
- Sensor monitoring
- Communication listeners
- Background processing

## Memory Management

### Monitoring Heap Usage

Watch the "Free heap" output to verify:
1. Heap decreases when tasks are created (normal)
2. Heap increases when tasks are deleted (resources freed)
3. Heap returns to baseline after deletion (no leaks)

### Expected Heap Pattern

```
Baseline:     ~280KB free
After +Task1: ~277KB free (-3KB stack)
After +Task2: ~275KB free (-2KB stack)  
After +Task3: ~272KB free (-3KB stack)
After -Task1: ~275KB free (+3KB recovered)
After -Task2: ~277KB free (+2KB recovered)
After -Task3: ~280KB free (+3KB recovered) ← Back to baseline
```

## Troubleshooting

**Task won't delete:**
- Ensure task has exited its main loop
- Check that task isn't blocked on a semaphore/queue
- Verify task name matches exactly

**Memory leak:**
- Ensure all dynamically allocated memory is freed before task exit
- Check that task cleans up resources (GPIO, peripherals)
- Monitor heap before and after deletion cycle

**Watchdog timeout:**
- Ensure `resetWatchdog()` is called in all task loops
- Don't block for long periods without resetting
- Increase watchdog timeout if necessary

## Experiments to Try

1. **Change task durations**: Modify iteration counts
2. **Add more workers**: Create additional task types
3. **Different patterns**: Implement event-driven task termination
4. **Error handling**: What happens if you delete a non-existent task?
5. **Resource limits**: How many tasks can you create simultaneously?
6. **Memory stress**: Create tasks with larger stacks and monitor heap

## Best Practices Demonstrated

✓ Always signal task completion before deletion  
✓ Give tasks time to exit gracefully  
✓ Monitor heap to detect memory leaks  
✓ Use clear naming conventions for tasks  
✓ Reset watchdog in all task loops  
✓ Clean up resources (GPIO, peripherals) before exit  
✓ Use volatile flags for inter-task communication  

## Real-World Applications

This pattern is useful for:
- **Dynamic workload**: Creating tasks on-demand
- **Resource management**: Freeing resources when not needed
- **Event-driven systems**: Spawning tasks for specific events
- **State machines**: Different tasks for different states
- **Power optimization**: Only run tasks when necessary

## Next Steps

- Combine with queues for task-to-task messaging
- Add error handling and recovery mechanisms
- Implement task priority inheritance
- Use semaphores for resource protection
- Study ESP-IDF event loop for advanced patterns
