# pinManager Library (ESP-IDF)

Named GPIO, PWM, tone, and ADC management for ESP-IDF.

## Overview

`pinManager` wraps common ESP32 pin workflows using string IDs so your application code can refer to pins by role (for example, `"led"`, `"buzzer"`, `"sensor"`) instead of hard-coded GPIO numbers.

## Dependencies

From `CMakeLists.txt`:

- `Counter`
- `esp_driver_gpio`
- `esp_driver_ledc`
- `esp_adc`

## Public API

```cpp
void digitalPin(std::string name, int8_t pin, gpio_mode_t mode = GPIO_MODE_INPUT);
int  digitalRead(std::string name);
void digitalWrite(std::string name, uint8_t value);

void pwmPin(std::string name, int8_t pin, uint32_t frequency = 5000,
            ledc_timer_t timer = LEDC_TIMER_0,
            ledc_timer_bit_t duty_resolution = LEDC_TIMER_13_BIT);
void setPwmDuty(std::string name, uint32_t duty);
void setPwmDutyPercent(std::string name, float percent);
void setPwmDutyPercent(std::string name, int8_t percent);
void setPwmDutyMicros(std::string name, uint32_t micros);
void setPwmFrequency(std::string name, uint32_t frequency);

void tone(std::string name, uint32_t frequency, uint8_t volume = 50, uint32_t duration_ms = 0);
void noTone(std::string name);
void update();

void analogPin(std::string name, int8_t pin);
int  analogRead(std::string name);
void analogWrite(std::string name, uint8_t value);
```

## Usage

```cpp
#include "pinManager.h"

pinManager pins;

extern "C" void app_main(void) {
    pins.digitalPin("led", 2, GPIO_MODE_OUTPUT);
    pins.pwmPin("buzzer", 25, 1000);
    pins.analogPin("sensor", 4);

    while (true) {
        pins.digitalWrite("led", 1);
        pins.tone("buzzer", 440, 35, 150);
        pins.update();

        int raw = pins.analogRead("sensor");
        (void)raw;

        vTaskDelay(pdMS_TO_TICKS(200));
        pins.digitalWrite("led", 0);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
```

## Behavior Notes

- Digital pin range validation is currently `0..39`.
- `digitalPin(...)` currently configures pull-down enabled and pull-up disabled.
- PWM channels are assigned sequentially each time `pwmPin(...)` is called.
- Timed tones require periodic `update()` calls.
- ADC helpers currently assume ADC1 GPIO range `1..10` (ESP32-S3 style mapping).

## Include

```cpp
#include "pinManager.h"
```
            pins.update();
            vTaskDelay(pdMS_TO_TICKS(10));
            elapsed += 10;
        }
        vTaskDelay(pdMS_TO_TICKS(50));  // Pause between notes
    }
}
```

## API Reference

### digitalPin()

```cpp
void digitalPin(std::string name, int8_t pin, gpio_mode_t mode = GPIO_MODE_INPUT)
```

Configure a GPIO pin for digital I/O.

**Parameters:**
- `name` - Unique identifier for the pin
- `pin` - GPIO number (0-39 for ESP32)
- `mode` - GPIO mode (default: GPIO_MODE_INPUT)
  - `GPIO_MODE_INPUT` - Input only
  - `GPIO_MODE_OUTPUT` - Output only
  - `GPIO_MODE_INPUT_OUTPUT` - Both input and output

**Behavior:**
- Validates pin number (0-39 for ESP32)
- Configures pull-down enabled, pull-up disabled by default
- Stores pin information in internal map

**Example:**
```cpp
pins.digitalPin("led", 2, GPIO_MODE_OUTPUT);
pins.digitalPin("button", 0, GPIO_MODE_INPUT);
```

### digitalRead()

```cpp
int digitalRead(std::string name)
```

Read the digital value of an input pin.

**Parameters:**
- `name` - Name of the pin to read

**Returns:** 
- `1` if pin is HIGH
- `0` if pin is LOW
- `-1` if pin not found or not configured for input

**Example:**
```cpp
int buttonState = pins.digitalRead("button");
if (buttonState == 1) {
    // Button pressed
}
```

### digitalWrite()

```cpp
void digitalWrite(std::string name, uint8_t value)
```

Write a digital value to an output pin.

**Parameters:**
- `name` - Name of the pin to write
- `value` - Value to write (0 = LOW, non-zero = HIGH)

**Behavior:**
- Only works if pin is configured as OUTPUT or INPUT_OUTPUT
- Silently ignores invalid pin names or input-only pins

**Example:**
```cpp
pins.digitalWrite("led", 1);  // Turn LED on
pins.digitalWrite("led", 0);  // Turn LED off
```

### pwmPin()

```cpp
void pwmPin(std::string name, int8_t pin, uint32_t frequency = 5000, 
            ledc_timer_t timer = LEDC_TIMER_0, 
            ledc_timer_bit_t duty_resolution = LEDC_TIMER_13_BIT)
```

Initialize a PWM channel on a GPIO pin.

**Parameters:**
- `name` - Unique identifier for the PWM pin
- `pin` - GPIO number (0-39 for ESP32)
- `frequency` - PWM frequency in Hz (default: 5000)
- `timer` - LEDC timer to use (LEDC_TIMER_0 to LEDC_TIMER_3)
- `duty_resolution` - Bit resolution (default: LEDC_TIMER_13_BIT = 8192 levels)

**Behavior:**
- Automatically assigns next available channel (0-7)
- Configures LEDC timer and channel
- Initial duty cycle is 0 (off)
- Uses LEDC_LOW_SPEED_MODE

**Note:** ESP32 has 8 PWM channels total. Exceeding this will cause errors.

**Example:**
```cpp
pins.pwmPin("led", 18, 5000);  // 5 kHz PWM
pins.pwmPin("servo", 19, 50);  // 50 Hz for servo
```

### setPwmDuty()

```cpp
void setPwmDuty(std::string name, uint32_t duty)
```

Set absolute PWM duty cycle value.

**Parameters:**
- `name` - Name of the PWM pin
- `duty` - Duty cycle value (0 to 8191 for 13-bit resolution)

**Example:**
```cpp
pins.setPwmDuty("led", 0);     // 0% (off)
pins.setPwmDuty("led", 4096);  // 50%
pins.setPwmDuty("led", 8191);  // 100% (full on)
```

### setPwmDutyPercent()

```cpp
void setPwmDutyPercent(std::string name, float percent)
void setPwmDutyPercent(std::string name, int8_t percent)
```

Set PWM duty cycle by percentage.

**Parameters:**
- `name` - Name of the PWM pin
- `percent` - Duty cycle percentage (0.0 to 100.0)

**Example:**
```cpp
pins.setPwmDutyPercent("led", 0);      // Off
pins.setPwmDutyPercent("led", 50);     // Half brightness
pins.setPwmDutyPercent("led", 100);    // Full brightness
pins.setPwmDutyPercent("led", 25.5);   // 25.5% (float overload)
```

### setPwmDutyMicros()

```cpp
void setPwmDutyMicros(std::string name, uint32_t micros)
```

Set PWM duty cycle by pulse width in microseconds (for servo control).

**Parameters:**
- `name` - Name of the PWM pin
- `micros` - Pulse width in microseconds

**Typical Servo Values:**
- 1000 µs = 0°
- 1500 µs = 90°
- 2000 µs = 180°

**Formula:** `duty = (micros × frequency × 8192) / 1000000`

**Example:**
```cpp
pins.pwmPin("servo", 18, 50);  // 50 Hz for servo
pins.setPwmDutyMicros("servo", 1000);  // 0 degrees
pins.setPwmDutyMicros("servo", 1500);  // 90 degrees
pins.setPwmDutyMicros("servo", 2000);  // 180 degrees
```

### setPwmFrequency()

```cpp
void setPwmFrequency(std::string name, uint32_t frequency)
```

Change the PWM frequency dynamically.

**Parameters:**
- `name` - Name of the PWM pin
- `frequency` - New frequency in Hz

**Note:** Changing frequency affects all channels using the same timer.

**Example:**
```cpp
pins.setPwmFrequency("led", 1000);   // Change to 1 kHz
pins.setPwmFrequency("led", 10000);  // Change to 10 kHz
```

### tone()

```cpp
void tone(std::string name, uint32_t frequency, uint8_t volume = 50, uint32_t duration_ms = 0)
```

Generate a tone at a specific frequency.

**Parameters:**
- `name` - Name of the PWM pin
- `frequency` - Tone frequency in Hz
- `volume` - Volume level 0-100% (default: 50)
  - 0 = silent
  - 50 = moderate volume
  - 100 = maximum volume (50% duty cycle)
- `duration_ms` - Duration in milliseconds (default: 0 = continuous)
  - 0 = play until `noTone()` is called
  - >0 = play for specified duration (requires calling `update()`)

**Behavior:**
- Sets PWM frequency to specified tone frequency
- Sets duty cycle based on volume (volume% of 8192 max duty)
- If duration specified, starts non-blocking timer

**Example:**
```cpp
// Continuous tone
pins.tone("buzzer", 440, 50);  // A4 note at 50% volume
vTaskDelay(pdMS_TO_TICKS(1000));
pins.noTone("buzzer");

// Timed tone (non-blocking)
pins.tone("buzzer", 262, 40, 500);  // C4 for 500ms
while(1) {
    pins.update();  // Required for timed tones
    vTaskDelay(pdMS_TO_TICKS(10));
}
```

### noTone()

```cpp
void noTone(std::string name)
```

Stop tone generation on a PWM pin.

**Parameters:**
- `name` - Name of the PWM pin

**Behavior:**
- Sets duty cycle to 0
- Stops any active timed tone

**Example:**
```cpp
pins.noTone("buzzer");
```

### update()

```cpp
void update()
```

Update method for handling timed tone completion. Must be called regularly in main loop.

**Behavior:**
- Checks all PWM pins with active timed tones
- Automatically calls `noTone()` when duration expires
- Should be called at least every 10-50ms for accurate timing

**Example:**
```cpp
void app_main() {
    pinManager pins;
    pins.pwmPin("buzzer", 25, 1000);
    
    // Play 1-second tone
    pins.tone("buzzer", 440, 50, 1000);
    
    // Main loop
    while(1) {
        pins.update();  // Handle tone timing
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

## Pin Limitations (ESP32)

### Valid GPIO Pins
- **Input/Output**: GPIO 0-19, 21-23, 25-27, 32-39
- **Output only**: GPIO 1-3 (shared with USB)
- **Input only**: GPIO 34-39 (no pull-up/pull-down)

### Reserved/Strapping Pins
Use with caution:
- **GPIO 0**: Boot mode selection
- **GPIO 2**: Boot mode selection
- **GPIO 5**: SD card
- **GPIO 12**: Boot voltage selection
- **GPIO 15**: Boot mode selection

### PWM Limitations
- Maximum 8 channels (16 channels in some modes)
- Channels share timers (4 timers available)
- All channels on same timer must use same frequency

## Dependencies

- ESP-IDF (v4.4 or later)
- Counter library (for tone timing)
- FreeRTOS (included with ESP-IDF)

## Troubleshooting

**GPIO not working:**
- Verify pin number is valid (0-39)
- Check if pin is reserved or strapping pin
- Ensure pin mode matches usage (input/output)

**PWM not working:**
- Maximum 8 channels - check if limit exceeded
- Verify timer assignment (max 4 timers)
- Check frequency is within valid range

**Tone issues:**
- Call `update()` regularly for timed tones
- Check buzzer/speaker connection and polarity
- Try different volume levels
- Verify frequency is audible (20-20000 Hz)

**Servo jitter:**
- Ensure stable power supply (servos draw current)
- Use 50 Hz frequency for standard servos
- Call `update()` regularly if using timed functions
- Consider external power for servos (not from ESP32)

## Examples

See the [PinManagerExample](../../examples/PinManagerExample) for a comprehensive demonstration.

## License

Open source - free to use and modify.
