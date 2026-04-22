# drvMotor Library (ESP-IDF)

ESP-IDF motor driver abstraction for dual DC motor control using common H-bridge drivers.

## Description

The drvMotor library provides a single class, motMgr, for managing left and right DC motors with two supported driver families:

- L293D (enable + direction pins)
- DRV8833 (direction/PWM input pins)

It defines clear pin mapping structures so application code can stay hardware-focused and readable.

## Supported Drivers

### L293D

Use this constructor and pin mapping when each motor uses an enable pin plus two direction pins.

```cpp
motMgr::pinL293D left = {.en = 18, .inA = 5, .inB = 17};
motMgr::pinL293D right = {.en = 19, .inA = 16, .inB = 4};
motMgr motors(left, right);
```

### DRV8833

Use this constructor and pin mapping when each motor uses two logic/PWM inputs.

```cpp
motMgr::pindrv8833 left = {.inA = 5, .inB = 17};
motMgr::pindrv8833 right = {.inA = 16, .inB = 4};
motMgr motors(left, right);
```

## API Reference

### Structs

```cpp
struct speedValue {
    int l;
    int r;
};
```

- Stores left and right speed values.

```cpp
struct motMgr::pinL293D {
    int en;
    int inA;
    int inB;
};
```

- Per-motor pin mapping for L293D.

```cpp
struct motMgr::pindrv8833 {
    int inA;
    int inB;
};
```

- Per-motor pin mapping for DRV8833.

### Constructors

```cpp
motMgr(pinL293D leftMotor, pinL293D rightMotor);
motMgr(pindrv8833 leftMotor, pindrv8833 rightMotor);
```

- Selects driver mode from constructor type.
- Stores left/right motor pin configuration.

### Methods

```cpp
void begin();
```

- Initializes motor control pins and runtime state.
- Call once after construction.

```cpp
void move(int16_t speedL, int16_t speedR);
```

- Sets left and right motor speed.
- Sign convention is typically:
  - positive: forward
  - negative: reverse
  - zero: stop

## Basic Usage

```cpp
#include "motorMgr.h"

extern "C" void app_main(void) {
    motMgr::pinL293D left = {.en = 18, .inA = 5, .inB = 17};
    motMgr::pinL293D right = {.en = 19, .inA = 16, .inB = 4};

    motMgr motors(left, right);
    motors.begin();

    motors.move(120, 120);   // forward
    vTaskDelay(pdMS_TO_TICKS(1000));

    motors.move(-120, -120); // reverse
    vTaskDelay(pdMS_TO_TICKS(1000));

    motors.move(0, 0);       // stop
}
```

## Component Integration

The component is CMake-ready and can be used as an ESP-IDF component from:

- ESP-IDF/libraries/drvMotor

Include header:

```cpp
#include "motorMgr.h"
```

## Current Status

Current source implementation includes:

- L293D constructor implementation in motorMgr.cpp

Public API declared in motorMgr.h includes additional constructors and methods intended for full motor control flow:

- DRV8833 constructor
- begin()
- move()

If these methods are not yet implemented in your current branch, implement them before using the full API at runtime.
