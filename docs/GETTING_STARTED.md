# Getting Started Guide

Welcome to the Development Board repository! This guide will help you get started with Arduino, ESP32, and other development boards.

## 📋 Table of Contents

1. [Hardware Requirements](#hardware-requirements)
2. [Software Setup](#software-setup)
3. [Quick Start](#quick-start)
4. [Building Your First Project](#building-your-first-project)
5. [Troubleshooting](#troubleshooting)

## 🔌 Hardware Requirements

### For Arduino Development

**Minimum Requirements:**
- Arduino board (Uno, Nano, Mega, or compatible)
- USB cable (Type A to Type B for Uno/Mega, or Type A to Mini-B for Nano)
- Computer (Windows, Mac, or Linux)

**Optional:**
- Breadboard
- Jumper wires
- LEDs and resistors
- Sensors and modules

### For ESP32 Development

**Minimum Requirements:**
- ESP32 development board (ESP32-DevKitC, ESP32-WROOM, or similar)
- USB cable (Micro-USB or USB-C, depending on your board)
- Computer (Windows, Mac, or Linux)

**Optional:**
- Breadboard
- Jumper wires
- WiFi network for connectivity examples
- Sensors and modules

## 💻 Software Setup

### Arduino IDE Setup

1. **Download Arduino IDE**
   - Visit [arduino.cc/en/software](https://www.arduino.cc/en/software)
   - Download the appropriate version for your OS
   - Install following the installer instructions

2. **Install Board Support** (for Arduino boards)
   - Arduino Uno, Nano, Mega are supported by default
   - For other boards, install via Tools > Board > Boards Manager

3. **Install ESP32 Support** (if using ESP32)
   - Open Arduino IDE
   - Go to File > Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools > Board > Boards Manager
   - Search for "ESP32" and install "ESP32 by Espressif Systems"

### Alternative: PlatformIO Setup

1. **Install VS Code**
   - Download from [code.visualstudio.com](https://code.visualstudio.com/)

2. **Install PlatformIO Extension**
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install

3. **PlatformIO automatically handles board support**

## 🚀 Quick Start

### Arduino Quick Start

1. **Connect Your Board**
   - Plug Arduino into your computer via USB
   - Wait for drivers to install (Windows may take a minute)

2. **Select Board and Port**
   - Open Arduino IDE
   - Tools > Board > Arduino AVR Boards > Arduino Uno (or your board)
   - Tools > Port > Select your COM port (COM3, COM4, etc.)

3. **Upload Your First Sketch**
   - Navigate to this repository's `Arduino/examples/Blink/`
   - Open `Blink.ino`
   - Click Upload button (→)
   - Watch the LED blink!

### ESP32 Quick Start

1. **Connect Your Board**
   - Plug ESP32 into your computer via USB
   - Wait for drivers to install

2. **Select Board and Port**
   - Open Arduino IDE (with ESP32 support installed)
   - Tools > Board > ESP32 Arduino > ESP32 Dev Module (or your board)
   - Tools > Port > Select your COM port

3. **Upload Your First Sketch**
   - Navigate to this repository's `ESP32/examples/BlinkESP32/`
   - Open `BlinkESP32.ino`
   - Click Upload button (→)
   - Watch the LED blink!

## 🔨 Building Your First Project

### Example: Temperature Monitor (Arduino)

1. **Hardware Setup**
   - Connect a temperature sensor (e.g., DHT11, DHT22, or DS18B20)
   - Refer to sensor datasheet for wiring

2. **Software**
   - Install required sensor library via Library Manager
   - Write or use example code to read temperature
   - Display on Serial Monitor or LCD

3. **Testing**
   - Upload code to Arduino
   - Open Serial Monitor (Tools > Serial Monitor)
   - Set baud rate to match your code
   - View temperature readings

### Example: WiFi Weather Station (ESP32)

1. **Hardware Setup**
   - ESP32 board only (uses built-in WiFi)
   - Optional: Add sensors for local readings

2. **Software**
   - Use WiFiScan example as a starting point
   - Add code to connect to your WiFi network
   - Fetch weather data from an API
   - Display information

3. **Testing**
   - Update WiFi credentials in code
   - Upload to ESP32
   - Open Serial Monitor
   - Verify WiFi connection and data retrieval

## 🔧 Troubleshooting

### Common Arduino Issues

**Issue: Port not detected**
- **Solution**: 
  - Check USB cable (some cables are power-only)
  - Install CH340 or FTDI drivers if using clone boards
  - Try a different USB port

**Issue: Upload fails**
- **Solution**:
  - Verify correct board is selected
  - Verify correct port is selected
  - Try pressing reset button during upload
  - Close Serial Monitor before uploading

**Issue: Code compiles but doesn't work**
- **Solution**:
  - Check wiring connections
  - Verify pin numbers in code match physical connections
  - Use Serial.println() for debugging

### Common ESP32 Issues

**Issue: Board not detected**
- **Solution**:
  - Install CP210x or CH340 USB drivers
  - Try different USB cable
  - Some boards require holding BOOT button during upload

**Issue: WiFi not connecting**
- **Solution**:
  - Verify SSID and password are correct
  - Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
  - Check WiFi router settings
  - Move closer to router

**Issue: Brown-out detector triggered**
- **Solution**:
  - Use higher quality USB cable
  - Use powered USB hub
  - Check power consumption of connected peripherals

### General Debugging Tips

1. **Use Serial Monitor**
   - Add Serial.println() statements to track code execution
   - Monitor variable values in real-time

2. **Check Connections**
   - Verify all wiring with a multimeter
   - Ensure good contact on breadboard

3. **Start Simple**
   - Test basic examples first (like Blink)
   - Add complexity incrementally

4. **Read Error Messages**
   - Compilation errors show line numbers
   - Google error messages for solutions

## 📚 Next Steps

- Explore more examples in the repository
- Try modifying existing examples
- Build your own projects
- Join Arduino/ESP32 communities online
- Check out tutorials and documentation

## 🔗 Useful Resources

### Arduino
- [Arduino Official Documentation](https://www.arduino.cc/reference/en/)
- [Arduino Forum](https://forum.arduino.cc/)
- [Arduino Project Hub](https://create.arduino.cc/projecthub)

### ESP32
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/)
- [Random Nerd Tutorials - ESP32](https://randomnerdtutorials.com/projects-esp32/)
- [ESP32 Forum](https://www.esp32.com/)

### General
- [Stack Overflow - Arduino Tag](https://stackoverflow.com/questions/tagged/arduino)
- [Stack Overflow - ESP32 Tag](https://stackoverflow.com/questions/tagged/esp32)
- [Adafruit Learning System](https://learn.adafruit.com/)
- [SparkFun Tutorials](https://learn.sparkfun.com/tutorials)

## 🎓 Learning Path

1. **Beginner**: Start with Blink and basic digital I/O
2. **Intermediate**: Learn analog I/O, serial communication, and sensors
3. **Advanced**: Explore WiFi/Bluetooth, web servers, and IoT projects
4. **Expert**: Create custom libraries, multi-device systems, and complex projects

Happy Making! 🚀
