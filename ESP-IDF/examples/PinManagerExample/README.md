# PinManager Example

This example demonstrates the capabilities of the `pinManager` library for ESP-IDF, showcasing GPIO management including digital I/O, PWM control, servo control, and tone generation.

## What This Example Does

The example demonstrates:
- Digital output (LED blinking)
- Digital input with pull-down resistor
- PWM generation with duty cycle control
- Servo motor control using microsecond pulse width
- Tone generation with volume and duration control
- Non-blocking tone playback

## Hardware Required

- ESP32 development board
- LED + 220Ω resistor (for digital output on GPIO 2)
- Buzzer or speaker (for tone generation on GPIO 25)
- Servo motor (optional, for PWM demo on GPIO 18)
- Push button (optional, for digital input on GPIO 32)

## Connections

```
GPIO 2  -> LED (+ 220Ω resistor) -> GND
GPIO 25 -> Buzzer/Speaker -> GND
GPIO 18 -> Servo signal wire (optional)
GPIO 32 -> Push button -> GND (with internal pull-down)
```

## How to Build and Flash

1. Navigate to this example directory:
   ```bash
   cd ESP-IDF/examples/PinManagerExample
   ```

2. Configure the project (optional):
   ```bash
   idf.py menuconfig
   ```

3. Build the project:
   ```bash
   idf.py build
   ```

4. Flash to your ESP32:
   ```bash
   idf.py flash
   ```

5. Monitor the output:
   ```bash
   idf.py monitor
   ```

## Example Output

```
I (xxx) PinExample: === Pin Manager Example ===
I (xxx) PinExample: ESP-IDF Version: v5.x.x
I (xxx) PinExample: Setting up pins...
I (xxx) PinExample: --- Digital Output Test ---
I (xxx) PinExample: LED ON
I (xxx) PinExample: LED OFF
I (xxx) PinExample: --- PWM Test ---
I (xxx) PinExample: PWM at 0%
I (xxx) PinExample: PWM at 25%
I (xxx) PinExample: PWM at 50%
I (xxx) PinExample: PWM at 75%
I (xxx) PinExample: PWM at 100%
I (xxx) PinExample: --- Servo Test ---
I (xxx) PinExample: Servo to 0 degrees
I (xxx) PinExample: Servo to 90 degrees
I (xxx) PinExample: Servo to 180 degrees
I (xxx) PinExample: --- Tone Test ---
I (xxx) PinExample: Playing C note (262 Hz)
I (xxx) PinExample: Playing E note (330 Hz)
I (xxx) PinExample: Playing G note (392 Hz)
```

## Key Concepts

### Digital I/O
- Configure pins as input or output using `digitalPin()`
- Read digital values with `digitalRead()`
- Write digital values with `digitalWrite()`

### PWM Control
- Initialize PWM on any GPIO with `pwmPin()`
- Set duty cycle by percentage (0-100%) with `setPwmDutyPercent()`
- Set absolute duty (0-8191 for 13-bit) with `setPwmDuty()`
- Control frequency with `setPwmFrequency()`

### Servo Control
- Use `setPwmDutyMicros()` to set pulse width in microseconds
- Typical servo range: 1000µs (0°) to 2000µs (180°)

### Tone Generation
- Generate tones with `tone(frequency, volume, duration_ms)`
- Control volume (0-100%)
- Optional duration for non-blocking timed tones
- Stop tones with `noTone()`
- Call `update()` regularly to handle timed tone completion

## Troubleshooting

- **No LED blinking**: Check GPIO pin number and wiring
- **PWM not working**: Verify you're not exceeding the 8-channel limit
- **Tone not playing**: Ensure buzzer/speaker is connected correctly
- **Servo jitter**: Check power supply stability

## See Also

- [pinManager Library Documentation](../../libraries/pinManager/README.md)
- [ESP-IDF GPIO API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html)
- [ESP-IDF LEDC (PWM) API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html)
