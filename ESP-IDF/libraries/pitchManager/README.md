# pitchManager Library (ESP-IDF)

Note and melody management utilities for buzzer playback via pinManager.

## Overview

This library provides:

- A frequency map for note names (`mNote`).
- `fNote` class to register melodies and play them on a configured PWM buzzer.
- Optional predefined melody initializer in `melodies.h`.

## Files

- `pitchManager.h`: public types and `fNote` API.
- `pitchManager.cpp`: melody storage and playback implementation.
- `melodies.h`: helper function `initializeMelodies(fNote&)` with predefined melodies.
- `melodyManager.cpp`: currently minimal placeholder.

## Core Types

```cpp
struct dtaMelody {
    std::string name;
    std::string alias;
    const int* melody;
    const int8_t* duration;
    size_t length;
    uint16_t tempo;
    uint8_t type;
};

class fNote {
public:
    void printNotes();

    void addMelody(std::string name, std::string alias, const int melody[], size_t length);
    void addMelody(std::string name, std::string alias, const std::string melody[], const int8_t duration[], size_t length);
    void addMelody(std::string name, std::string alias, const int melody[], const int8_t duration[], size_t length);

    void deleteMelody(std::string name);
    dtaMelody getMelody(std::string name);

    void playMelody(std::string name,
                    pinManager& pinMgr,
                    std::string buzzerId,
                    uint8_t volume = 50,
                    int16_t speed = -1,
                    bool reverse = false);
};
```

## Usage

```cpp
#include "pitchManager.h"
#include "melodies.h"
#include "pinManager.h"

extern "C" void app_main(void) {
    pinManager pins;
    pins.pwmPin("buzzer", 25, 1000);

    fNote notes;
    initializeMelodies(notes);

    notes.playMelody("start", pins, "buzzer", 40);
}
```

## Playback Behavior

- Tempo is converted to whole-note duration and per-note timing.
- Positive duration means normal note length; negative values are dotted notes.
- Each note plays for ~90% of computed duration, followed by ~10% silence.
- `reverse=true` plays the melody from end to start.

## Current Limitations

- There is no `CMakeLists.txt` in this folder yet; add one before using as an ESP-IDF component directly.
- Some `addMelody(...)` overloads allocate arrays and keep raw pointers without ownership cleanup in destructor.
- `const int*` / `const int8_t*` storage means lifetime must outlive playback for caller-provided arrays.
- Header includes `counter.h`; ensure include path/casing matches your component setup.

## Include

```cpp
#include "pitchManager.h"
```
