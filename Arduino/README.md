# Arduino Code and Libraries

This directory contains Arduino-specific code, libraries, and examples.

## 📁 Structure

- **examples/** - Example sketches demonstrating various Arduino features
- **libraries/** - Custom Arduino libraries for reusable code

## 🚀 Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (version 1.8.0 or later)
- OR [Arduino CLI](https://arduino.github.io/arduino-cli/)
- Compatible Arduino board (Uno, Nano, Mega, etc.)
- USB cable for programming

### Using Examples

1. Open Arduino IDE
2. Navigate to File > Open
3. Browse to the example folder you want to use
4. Open the `.ino` file
5. Select your board: Tools > Board > [Your Arduino Board]
6. Select your port: Tools > Port > [Your COM Port]
7. Click Upload

### Using Libraries

To use custom libraries from this repository:

1. Copy the library folder to your Arduino libraries directory:
   - Windows: `Documents\Arduino\libraries\`
   - Mac: `Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
2. Restart Arduino IDE
3. Include the library in your sketch: `#include <LibraryName.h>`

## 📚 Available Examples

### Blink
Basic LED blinking example demonstrating digital output.

**Hardware Required:**
- Arduino board
- Built-in LED (or external LED with 220Ω resistor)

### CounterExample
Demonstrates the Counter library with counting and timing functionality.

**Hardware Required:**
- Arduino board
- Built-in LED

**Features:**
- Event counting with cCount class
- Timing operations with cTime class
- Flag management
- Callback functionality
- Serial output monitoring

### SerialCommunication
Demonstrates serial communication between Arduino and computer.

**Hardware Required:**
- Arduino board
- USB connection

## 🔧 Supported Boards

- Arduino Uno
- Arduino Nano
- Arduino Mega
- Arduino Leonardo
- Other Arduino-compatible boards

## 📝 Creating Your Own Examples

When creating new examples:
1. Create a new folder in `examples/`
2. Name the `.ino` file the same as the folder name
3. Include clear comments explaining the code
4. Document required hardware and connections
5. Add a README if the example is complex

## 🤝 Contributing

Contributions are welcome! Please ensure:
- Code follows Arduino coding style
- Examples include proper documentation
- Libraries include header comments and usage examples
