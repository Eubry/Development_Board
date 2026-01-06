# ESP32 Code and Libraries

This directory contains ESP32-specific code, libraries, and examples.

## 📁 Structure

- **examples/** - Example sketches demonstrating various ESP32 features
- **libraries/** - Custom ESP32 libraries for reusable code

## 🚀 Getting Started

### Prerequisites

**Option 1: Arduino IDE with ESP32 Support**
1. Install [Arduino IDE](https://www.arduino.cc/en/software) (version 1.8.0 or later)
2. Add ESP32 board support:
   - Open Arduino IDE
   - Go to File > Preferences
   - Add to "Additional Board Manager URLs": 
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools > Board > Boards Manager
   - Search for "ESP32" and install "ESP32 by Espressif Systems"

**Option 2: ESP-IDF**
- Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) development framework
- Follow official ESP-IDF setup guide

**Option 3: PlatformIO**
- Install [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)
- PlatformIO automatically handles ESP32 toolchain

### Hardware

- ESP32 development board (ESP32-DevKitC, ESP32-WROOM, etc.)
- USB cable (usually micro-USB or USB-C depending on your board)

### Using Examples

1. Open Arduino IDE (with ESP32 support installed)
2. Navigate to File > Open
3. Browse to the example folder you want to use
4. Open the `.ino` file
5. Select your ESP32 board: Tools > Board > ESP32 Arduino > [Your ESP32 Board]
6. Select your port: Tools > Port > [Your COM Port]
7. Click Upload

## 📚 Available Examples

### BlinkESP32
Basic LED blinking example for ESP32.

**Hardware Required:**
- ESP32 development board
- Built-in LED (usually on GPIO2)

**Features:**
- Digital output control
- Serial debugging

### CounterExample
Demonstrates the Counter library with high-precision ESP32 timing.

**Hardware Required:**
- ESP32 development board
- Built-in LED (GPIO2)

**Features:**
- High-precision microsecond timing using esp_timer
- Event counting with cCount class
- Precise timing with cTime class
- Flag management
- Callback functionality
- Real-time serial monitoring

### WiFiScan
Scans and displays available WiFi networks.

**Hardware Required:**
- ESP32 development board

**Features:**
- WiFi scanning
- RSSI measurement
- Network information display

## 🌟 ESP32 Capabilities

The ESP32 is a powerful microcontroller with:
- **WiFi**: 2.4 GHz 802.11 b/g/n
- **Bluetooth**: Classic BT and BLE (Bluetooth Low Energy)
- **Dual Core**: Two Xtensa 32-bit LX6 processors
- **GPIO**: Up to 34 programmable pins
- **ADC**: 18 channels, 12-bit resolution
- **DAC**: 2 channels, 8-bit resolution
- **PWM**: 16 channels
- **Peripherals**: SPI, I2C, I2S, UART, CAN, etc.

## 🔧 Common ESP32 Pins

| Function | GPIO Pin(s) |
|----------|-------------|
| Built-in LED | GPIO2 (most boards) |
| I2C SDA | GPIO21 |
| I2C SCL | GPIO22 |
| SPI MOSI | GPIO23 |
| SPI MISO | GPIO19 |
| SPI SCK | GPIO18 |
| SPI SS | GPIO5 |

**Note:** Pin assignments may vary by board manufacturer. Always check your specific board's pinout diagram.

## 📝 Creating Your Own Examples

When creating new examples:
1. Create a new folder in `examples/`
2. Name the `.ino` file the same as the folder name
3. Include clear comments explaining the code
4. Document required hardware and connections
5. Specify any required WiFi credentials or configurations
6. Add a README if the example is complex

## 🤝 Contributing

Contributions are welcome! Please ensure:
- Code follows ESP32/Arduino coding style
- Examples include proper documentation
- WiFi credentials are not hardcoded (use placeholders)
- Libraries include header comments and usage examples

## 📖 Resources

- [ESP32 Official Documentation](https://docs.espressif.com/projects/esp-idf/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
