# Common Libraries

This directory contains platform-independent or cross-compatible libraries that can be used across different development boards.

## 📁 Structure

- **libraries/** - Shared libraries usable on Arduino, ESP32, and other compatible boards

## 🎯 Purpose

Common libraries are designed to:
- Provide reusable code across different platforms
- Reduce code duplication
- Offer standardized functionality
- Simplify cross-platform development

## 📚 Available Libraries

### Counter
A powerful counter and timer library providing precise counting and timing functionality.

**Features:**
- **cCount**: Count-based event management with flag control
- **cTime**: Microsecond-precision timing for ESP32 (millisecond fallback for other platforms)
- Automatic goal change detection
- Start/finish state tracking
- Callback support with templates
- Reset functionality

**Classes:**
- `cCount` - Event counter with configurable goals and flag management
- `cTime` - High-precision timer with microsecond accuracy (ESP32)

**Example:**
```cpp
#include "counter.h"

cCount counter;
cTime timer;

void loop() {
  counter.count(10);  // Count to 10
  if (counter.finish()) {
    // Execute when count reaches 10
  }
  
  timer.wait(1000000);  // Wait 1 second (microseconds)
  if (timer.finish()) {
    // Execute every second
  }
}
```

### Utils
A utility library providing common functions for all platforms.

**Features:**
- Value mapping and constraining
- Array averaging
- Timing utilities
- Cross-platform compatible

See [Utils/README.md](libraries/Utils/README.md) for detailed documentation.

## 🔧 Using Common Libraries

### Arduino IDE

1. Copy the desired library folder to your Arduino libraries directory:
   - Windows: `Documents\Arduino\libraries\`
   - Mac: `Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
2. Restart Arduino IDE
3. Include in your sketch:
   ```cpp
   #include <Utils.h>
   ```

### PlatformIO

Add to your `platformio.ini`:
```ini
lib_deps = 
    Utils
```

Or use a local path:
```ini
lib_extra_dirs = ../Common/libraries
```

### ESP-IDF

Add the library to your component path or copy to your project's components directory.

## 💡 Best Practices

When creating common libraries:

1. **Platform Independence**: Use standard C++ features when possible
2. **Compatibility**: Test on multiple platforms (Arduino, ESP32, etc.)
3. **Documentation**: Include clear header comments and usage examples
4. **Namespace**: Use namespaces to avoid naming conflicts
5. **Templates**: Use templates for type-flexible functions
6. **Dependencies**: Minimize external dependencies

## 🤝 Contributing

When contributing common libraries:
- Ensure code works on both Arduino and ESP32 platforms
- Include comprehensive documentation
- Add usage examples
- Test thoroughly on different boards
- Follow C++ best practices
- Use header guards in `.h` files

## 📝 Library Structure

A typical common library should include:

```
LibraryName/
├── LibraryName.h          # Header file with function declarations
├── LibraryName.cpp        # Implementation file (if needed)
├── README.md              # Library documentation
├── examples/              # Usage examples (optional)
│   └── Example/
│       └── Example.ino
└── keywords.txt           # Arduino IDE syntax highlighting (optional)
```

## 🔍 Example Usage

```cpp
#include <Arduino.h>
#include <Utils.h>

void setup() {
  Serial.begin(9600);
  
  // Use common library functions
  int mapped = Utils::mapValue(512, 0, 1023, 0, 255);
  int constrained = Utils::constrainValue(300, 0, 255);
  
  Serial.print("Mapped: ");
  Serial.println(mapped);
  Serial.print("Constrained: ");
  Serial.println(constrained);
}

void loop() {
  // Your code here
}
```

## 📖 Resources

- [Arduino Library Specification](https://arduino.github.io/arduino-cli/latest/library-specification/)
- [PlatformIO Library Management](https://docs.platformio.org/en/latest/librarymanager/)
- [C++ Best Practices](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
