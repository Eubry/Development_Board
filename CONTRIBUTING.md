# Contributing to Development Board

Thank you for your interest in contributing to this project! This document provides guidelines for contributing code, libraries, and examples.

## 🤝 How to Contribute

### Reporting Issues

- Check if the issue already exists
- Provide clear description of the problem
- Include board type, IDE version, and error messages
- Add code snippets if relevant

### Suggesting Features

- Open an issue with the "enhancement" label
- Describe the feature and its use case
- Explain why it would be useful

### Contributing Code

1. **Fork the Repository**
   - Click the "Fork" button on GitHub
   - Clone your fork locally

2. **Create a Branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make Your Changes**
   - Follow the coding standards (see below)
   - Test your code thoroughly
   - Update documentation as needed

4. **Commit Your Changes**
   ```bash
   git add .
   git commit -m "Add feature: description"
   ```

5. **Push to Your Fork**
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create a Pull Request**
   - Go to the original repository
   - Click "New Pull Request"
   - Select your branch
   - Provide clear description of changes

## 📝 Coding Standards

### General Guidelines

- Write clear, readable code
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and concise

### C++ / Arduino Style

```cpp
// Use camelCase for variables and functions
int sensorValue = 0;
void readSensor() { }

// Use UPPER_CASE for constants
const int LED_PIN = 13;
const int MAX_SENSORS = 5;

// Use descriptive names
int temperature;        // Good
int t;                 // Avoid

// Add comments for clarity
// Initialize the temperature sensor on pin A0
int tempSensor = A0;
```

### File Organization

**For Examples:**
```
ExampleName/
├── ExampleName.ino    # Main sketch file
└── README.md          # Optional: detailed documentation
```

**For Libraries:**
```
LibraryName/
├── LibraryName.h      # Header file
├── LibraryName.cpp    # Implementation (if needed)
├── README.md          # Documentation
├── examples/          # Usage examples
│   └── Example/
│       └── Example.ino
└── keywords.txt       # Arduino IDE syntax highlighting
```

### Documentation

- **Code Comments**: Explain why, not what
- **Function Documentation**: Describe parameters and return values
- **README Files**: Include purpose, usage, and examples

Example header documentation:
```cpp
/**
 * @brief Read temperature from sensor
 * @param pin Analog pin number where sensor is connected
 * @return Temperature in Celsius
 */
float readTemperature(int pin);
```

## 🧪 Testing

### Before Submitting

- Test on actual hardware when possible
- Verify code compiles without errors
- Check for memory leaks on embedded devices
- Test on multiple board types if applicable

### Arduino Testing
```bash
# Using Arduino CLI
arduino-cli compile --fqbn arduino:avr:uno YourSketch/
```

### ESP32 Testing
```bash
# Using Arduino CLI with ESP32
arduino-cli compile --fqbn esp32:esp32:esp32 YourSketch/
```

## 📂 Project Structure

When adding new content:

- **Arduino examples** → `Arduino/examples/`
- **Arduino libraries** → `Arduino/libraries/`
- **ESP32 examples** → `ESP32/examples/`
- **ESP32 libraries** → `ESP32/libraries/`
- **Cross-platform libraries** → `Common/libraries/`
- **Documentation** → `docs/`

## ✅ Pull Request Checklist

Before submitting a PR, ensure:

- [ ] Code follows the project's coding style
- [ ] All files are properly formatted
- [ ] Documentation is updated (README, comments, etc.)
- [ ] Code has been tested on actual hardware
- [ ] No unnecessary files are included (build artifacts, IDE files)
- [ ] Commit messages are clear and descriptive
- [ ] PR description explains what and why

## 🎯 What We're Looking For

### Examples
- Well-documented code
- Clear hardware requirements
- Practical, educational value
- Tested and working

### Libraries
- Reusable, modular code
- Clear API documentation
- Usage examples
- Compatible with standard IDEs

### Documentation
- Clear, concise writing
- Helpful for beginners and advanced users
- Accurate technical information
- Good formatting

## 🚫 What to Avoid

- Hardcoded credentials (WiFi passwords, API keys, etc.)
- Copyrighted code without permission
- Untested code
- Poor documentation
- Binary files (unless necessary)
- IDE-specific configuration files

## 💡 Tips for Contributors

### For Beginners
- Start with simple examples
- Fix typos in documentation
- Improve existing examples
- Ask questions if unsure

### For Experienced Developers
- Add advanced examples
- Create reusable libraries
- Optimize existing code
- Help review other PRs

## 📞 Getting Help

- Open an issue for questions
- Check existing documentation
- Join community discussions
- Be respectful and patient

## 📜 Code of Conduct

### Our Standards

- Be respectful and inclusive
- Accept constructive criticism
- Focus on what's best for the community
- Show empathy towards others

### Unacceptable Behavior

- Harassment or discrimination
- Trolling or insulting comments
- Publishing others' private information
- Unprofessional conduct

## 🏆 Recognition

Contributors will be:
- Listed in project credits
- Acknowledged in release notes
- Appreciated by the community!

## 📄 License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to Development Board! 🚀
