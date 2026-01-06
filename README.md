# Development Board Code & Libraries

This repository contains code and libraries for various development boards including Arduino, ESP32, and other microcontroller platforms. The project is primarily written in C++.

## 📁 Repository Structure

```
Development_Board/
├── Arduino/              # Arduino-specific code and libraries
│   ├── examples/         # Example projects and sketches
│   └── libraries/        # Custom Arduino libraries
├── ESP32/                # ESP32-specific code and libraries
│   ├── examples/         # Example projects
│   └── libraries/        # Custom ESP32 libraries
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

1. Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) or use Arduino IDE with ESP32 board support
2. Navigate to the `ESP32/` directory
3. Browse `examples/` for sample projects
4. Use libraries from `libraries/` in your projects

### Common Libraries

The `Common/libraries/` directory contains platform-independent or cross-compatible code that can be used across different development boards.

## 📚 Documentation

Additional documentation, datasheets, and guides can be found in the `docs/` directory.

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
