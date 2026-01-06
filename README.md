# Development Board Code & Libraries

This repository contains code and libraries for various development boards including Arduino, ESP32, and other microcontroller platforms. The project is primarily written in C++.

## 📁 Repository Structure

```
Development_Board/
├── Arduino/              # Arduino-specific code and libraries
│   ├── examples/         # Example projects and sketches
│   └── libraries/        # Custom Arduino libraries
├── ESP32/                # ESP32-specific code and libraries (Arduino framework)
│   ├── examples/         # Example projects
│   └── libraries/        # Custom ESP32 libraries
├── ESP-IDF/              # ESP-IDF framework specific code and libraries
│   ├── examples/         # ESP-IDF example projects
│   └── libraries/        # ESP-IDF specific libraries (FreeRTOS, advanced features)
├── Common/               # Shared code and libraries
│   └── libraries/        # Common libraries usable across platforms
└── docs/                 # Documentation and guides
```

## 🚀 Getting Started

### Arduino Development

1. Install [Arduino IDE](https://www.arduino.cc/en/software) or [Arduino CLI](https://arduino.github.io/arduino-cli/)
2. Navigate to the `Arduino/` directory
3. Browse `examples/` for sample projects
4. Install libraries from `libraries/` by copying to your Arduino libraries folder

### ESP32 Development

**Option 1: Arduino Framework (Easier)**
1. Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) or use Arduino IDE with ESP32 board support
2. Navigate to the `ESP32/` directory
3. Browse `examples/` for sample projects
4. Use libraries from `libraries/` in your projects

**Option 2: ESP-IDF Framework (Advanced)**
1. Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) (version 4.4 or later)
2. Set up the ESP-IDF environment
3. Navigate to the `ESP-IDF/` directory
4. Copy libraries to your project's `components/` directory
5. Use advanced FreeRTOS features and ESP32 capabilities

### Common Libraries

The `Common/libraries/` directory contains platform-independent or cross-compatible code that can be used across different development boards.

## 📚 Documentation

### Library Documentation

- **[Common Libraries](Common/README.md)** - Cross-platform libraries (Counter, Utils)
- **[Arduino Examples](Arduino/README.md)** - Arduino-specific examples and guides
- **[ESP32 Examples](ESP32/README.md)** - ESP32 Arduino framework examples
- **[ESP-IDF Libraries & Examples](ESP-IDF/README.md)** - Advanced ESP-IDF development

### Guides

Additional documentation, datasheets, and guides can be found in the `docs/` directory.

## 📦 Available Libraries

### Common Libraries (Cross-Platform)

**Counter** - Counting and timing library
- Event counter with goal-based triggers
- High-precision microsecond timer (ESP32)
- Flag management for state transitions
- Callback support

**Utils** - Utility functions
- Value mapping and constraining
- Array averaging
- Template-based for type flexibility

### ESP-IDF Libraries (ESP32 Advanced)

**Utils (ESP-IDF)** - FreeRTOS task management
- Advanced task manager class
- Multi-core task assignment
- Priority and stack size control
- Watchdog integration
- Dynamic task lifecycle management

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests with:
- New example projects
- Custom libraries
- Bug fixes
- Documentation improvements

## 📝 License

This project is open source. Please check individual files for specific license information.

## 🔧 Supported Boards

- Arduino Uno, Nano, Mega
- ESP32 (various modules)
- More boards can be added in the future
