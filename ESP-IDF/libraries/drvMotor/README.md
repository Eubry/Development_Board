# drvMotor Library (ESP-IDF)

Dual DC motor abstraction (`motMgr`) for L293D and DRV8833 style wiring.

## Overview

This library provides a single class, `motMgr`, with driver-specific pin structs and a shared control API:

- `begin()` for pin setup
- `move(speedL, speedR)` for differential speed commands

## Dependencies

From `CMakeLists.txt`:

- `freertos`
- `esp_system`
- `esp_timer`
- `pinManager`

## Public API

```cpp
struct motMgr::pinL293D { int en; int inA; int inB; };
struct motMgr::pindrv8833 { int inA; int inB; };

motMgr(pinL293D leftMotor, pinL293D rightMotor);
motMgr(pindrv8833 leftMotor, pindrv8833 rightMotor);

void begin();
void move(int16_t speedL, int16_t speedR);
```

## Usage

```cpp
#include "motorMgr.h"

extern "C" void app_main(void) {
    motMgr::pinL293D left = {.en = 18, .inA = 5, .inB = 17};
    motMgr::pinL293D right = {.en = 19, .inA = 16, .inB = 4};

    motMgr motors(left, right);
    motors.begin();

    motors.move(60, 60);
    vTaskDelay(pdMS_TO_TICKS(1000));
    motors.move(0, 0);
}
```

## Behavior Notes

- The class tracks previous left/right speed values and only updates outputs when a side changes speed.
- Positive/negative speed is used as direction selection in implementation.

## Current Implementation Caveats

- Constructor for L293D sets internal `driver` to `"L293D"` but `begin()` checks `"l293d"`; this currently prevents L293D setup path from executing as written.
- Some output calls in `motorMgr.cpp` still use Arduino-style `analogWrite`/`digitalWrite` on raw pins, while other paths use `pinManager` named pins.
- Because of the above, treat this component as in-progress and validate behavior on target hardware before production use.

## Include

```cpp
#include "motorMgr.h"
```
