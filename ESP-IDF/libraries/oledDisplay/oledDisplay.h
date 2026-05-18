#pragma once
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <cstdint>
#include <vector>
#include <string>
    #include <driver/i2c_master.h>

class OLEDDisplay {
public:
    // Constructor with optional pin configuration
    OLEDDisplay(gpio_num_t sda = GPIO_NUM_8, gpio_num_t scl = GPIO_NUM_9, uint8_t addr = 0x3C);
    
    // Destructor
    ~OLEDDisplay();
    
    // Initialize the display
    bool begin();
    
    // Clear the display
    void clear();
    
    // Turn display on/off
    void displayOn(bool on);
    
    // Set contrast (0-255)
    void setContrast(uint8_t contrast);
    
    // Draw a single character at position (x, y)
    void drawChar(uint8_t x, uint8_t y, char c);
    
    // Draw a string at position (x, y)
    void drawString(uint8_t x, uint8_t y, const std::string& text);
    
    // Draw a pixel at position (x, y)
    void drawPixel(uint8_t x, uint8_t y, bool on);
    
    // Draw a horizontal line
    void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool on);
    
    // Draw a rectangle
    void drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool filled, bool on);
    
    // Draw a circle
    void drawCircle(uint8_t x, uint8_t y, uint8_t radius, bool filled, bool on);

    // Draw a sensor status circle: outline when false, filled when true
    void drawSensorCircle(uint8_t x, uint8_t y, uint8_t radius, bool active);
    
    // Update display (write buffer to device)
    void update();
    
    // Set invert display colors
    void invertDisplay(bool invert);
    
    // Get display width and height
    uint8_t getWidth() const { return WIDTH; }
    uint8_t getHeight() const { return HEIGHT; }

private:
    // Display dimensions
    static constexpr uint8_t WIDTH = 128;
    static constexpr uint8_t HEIGHT = 64;
    static constexpr uint8_t PAGES = HEIGHT / 8;
    
    // I2C configuration
    static constexpr i2c_port_t I2C_PORT = I2C_NUM_0;
    static constexpr uint32_t I2C_FREQ = 400000; // 400 kHz
    
    // I2C pin and address configuration (configurable per instance)
    gpio_num_t sda_pin;
    gpio_num_t scl_pin;
    uint8_t i2c_address;
    
    // I2C master bus and device handles
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
    
    // Display buffer (128x64 / 8 = 1024 bytes)
    std::vector<uint8_t> buffer;
    
    // Helper functions
    bool writeCommand(uint8_t cmd);
    bool writeData(const uint8_t* data, size_t len);
    void setPageAndColumn(uint8_t page, uint8_t column);
    
    // SSD1306 command definitions
    static constexpr uint8_t SET_CONTRAST = 0x81;
    static constexpr uint8_t DISPLAY_ON = 0xAF;
    static constexpr uint8_t DISPLAY_OFF = 0xAE;
    static constexpr uint8_t NORMAL_DISPLAY = 0xA6;
    static constexpr uint8_t INVERTED_DISPLAY = 0xA7;
    static constexpr uint8_t SET_DISPLAY_OFFSET = 0xD3;
    static constexpr uint8_t SET_START_LINE = 0x40;
    static constexpr uint8_t SET_SEGMENT_REMAP = 0xA1;
    static constexpr uint8_t SET_COM_OUTPUT_DIRECTION = 0xC8;
    static constexpr uint8_t SET_COM_PIN_CONFIG = 0xDA;
    static constexpr uint8_t SET_DISPLAY_CLOCK = 0xD5;
    static constexpr uint8_t SET_PRECHARGE_PERIOD = 0xD9;
    static constexpr uint8_t SET_VCOMH_LEVEL = 0xDB;
    static constexpr uint8_t SET_CHARGE_PUMP = 0x8D;
    static constexpr uint8_t SET_PAGE_ADDRESS = 0xB0;
    static constexpr uint8_t SET_COLUMN_ADDRESS_LOW = 0x00;
    static constexpr uint8_t SET_COLUMN_ADDRESS_HIGH = 0x10;
    static constexpr uint8_t SET_MEMORY_ADDRESSING = 0x20;
};

#endif // OLED_DISPLAY_H
