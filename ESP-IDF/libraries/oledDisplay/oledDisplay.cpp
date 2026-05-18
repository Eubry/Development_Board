#include "oledDisplay.h"
#include <cstring>
#include <algorithm>
#include <esp_log.h>

static const char* TAG = "OLEDDisplay";

// Font data for basic ASCII characters (5x7 font)
static const uint8_t fontData[96][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x59, 0x51, 0x3E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x7F, 0x41, 0x00, 0x00}, // [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // Backslash
    {0x00, 0x41, 0x7F, 0x00, 0x00}, // ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    {0x78, 0x04, 0x18, 0x04, 0x78}, // m
    {0x78, 0x04, 0x04, 0x04, 0x78}, // n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // }
    {0x10, 0x08, 0x08, 0x10, 0x08}, // ~
};

OLEDDisplay::OLEDDisplay(gpio_num_t sda, gpio_num_t scl, uint8_t addr)
    : sda_pin(sda), scl_pin(scl), i2c_address(addr) {
    bus_handle = nullptr;
    dev_handle = nullptr;
    buffer.resize(WIDTH * PAGES, 0);
}

OLEDDisplay::~OLEDDisplay() {
       if (dev_handle) {
           i2c_master_bus_rm_device(dev_handle);
       }
       if (bus_handle) {
           i2c_del_master_bus(bus_handle);
    }
}

bool OLEDDisplay::begin() {
    esp_err_t ret;
    
       // Initialize I2C master bus
       i2c_master_bus_config_t bus_cfg = {};
       bus_cfg.i2c_port = I2C_NUM_0;
       bus_cfg.sda_io_num = sda_pin;
       bus_cfg.scl_io_num = scl_pin;
       bus_cfg.clk_source = I2C_CLK_SRC_DEFAULT;
       bus_cfg.glitch_ignore_cnt = 7;
       bus_cfg.intr_priority = 0;
       bus_cfg.trans_queue_depth = 0;
       bus_cfg.flags.enable_internal_pullup = true;
       
       ret = i2c_new_master_bus(&bus_cfg, &bus_handle);
       if (ret != ESP_OK) {
           ESP_LOGE(TAG, "Failed to create I2C bus: %s", esp_err_to_name(ret));
           return false;
       }
       
       // Add I2C device
       i2c_device_config_t dev_cfg = {};
       dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
       dev_cfg.device_address = i2c_address;
       dev_cfg.scl_speed_hz = I2C_FREQ;
       
       ret = i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);
       if (ret != ESP_OK) {
           ESP_LOGE(TAG, "Failed to add I2C device: %s", esp_err_to_name(ret));
           i2c_del_master_bus(bus_handle);
           return false;
       }
    
    // SSD1306 Initialization Sequence
    uint8_t init_sequence[] = {
        DISPLAY_OFF,               // 0xAE - Turn display off
        SET_DISPLAY_CLOCK,         // 0xD5
        0x80,                      // Clock divide ratio and oscillator frequency
        SET_DISPLAY_OFFSET,        // 0xD3
        0x00,                      // Display offset
        SET_START_LINE | 0x00,     // 0x40 - Set start line address
        SET_CHARGE_PUMP,           // 0x8D
        0x14,                      // Enable charge pump
        SET_MEMORY_ADDRESSING,     // 0x20
        0x02,                      // Page addressing mode
        SET_SEGMENT_REMAP,         // 0xA1 - Remap columns
        SET_COM_OUTPUT_DIRECTION,  // 0xC8 - Remap rows
        SET_COM_PIN_CONFIG,        // 0xDA
        0x12,                      // COM pins configuration
        SET_CONTRAST,              // 0x81
        0x80,                      // Contrast value
        SET_PRECHARGE_PERIOD,      // 0xD9
        0xF1,                      // Precharge period
        SET_VCOMH_LEVEL,           // 0xDB
        0x40,                      // VCOMH level
        NORMAL_DISPLAY,            // 0xA6 - Normal display
        DISPLAY_ON,                // 0xAF - Turn display on
    };
    
    for (size_t i = 0; i < sizeof(init_sequence); i++) {
        if (!writeCommand(init_sequence[i])) {
               i2c_master_bus_rm_device(dev_handle);
               i2c_del_master_bus(bus_handle);
            return false;
        }
    }
    
    clear();
    update();
    
    ESP_LOGI(TAG, "OLED display initialized successfully");
    return true;
}

bool OLEDDisplay::writeCommand(uint8_t cmd) {
    uint8_t data[] = {0x00, cmd};  // Control byte: 0x00 for command
    return writeData(data, sizeof(data));
}

bool OLEDDisplay::writeData(const uint8_t* data, size_t len) {
       esp_err_t ret = i2c_master_transmit(dev_handle, data, len, -1);
       if (ret != ESP_OK) {
           ESP_LOGE(TAG, "I2C write failed: %s", esp_err_to_name(ret));
           return false;
       }
       return true;
}

void OLEDDisplay::clear() {
    std::fill(buffer.begin(), buffer.end(), 0);
}

void OLEDDisplay::displayOn(bool on) {
    writeCommand(on ? DISPLAY_ON : DISPLAY_OFF);
}

void OLEDDisplay::setContrast(uint8_t contrast) {
    writeCommand(SET_CONTRAST);
    writeCommand(contrast);
}

void OLEDDisplay::invertDisplay(bool invert) {
    writeCommand(invert ? INVERTED_DISPLAY : NORMAL_DISPLAY);
}

void OLEDDisplay::drawPixel(uint8_t x, uint8_t y, bool on) {
    if (x >= WIDTH || y >= HEIGHT) {
        return;
    }
    
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    
    if (on) {
        buffer[page * WIDTH + x] |= (1 << bit);
    } else {
        buffer[page * WIDTH + x] &= ~(1 << bit);
    }
}

void OLEDDisplay::drawChar(uint8_t x, uint8_t y, char c) {
    if (x + 5 >= WIDTH || y + 7 >= HEIGHT) {
        return;
    }
    
    // Handle out-of-bounds ASCII
    uint8_t idx = (uint8_t)c;
    if (idx < 32 || idx >= 128) {
        idx = 32;  // Default to space
    }
    idx -= 32;
    
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t byte = fontData[idx][i];
        for (uint8_t j = 0; j < 8; j++) {
            if (byte & (1 << j)) {
                drawPixel(x + i, y + j, true);
            }
        }
    }
}

void OLEDDisplay::drawString(uint8_t x, uint8_t y, const std::string& text) {
    uint8_t current_x = x;
    
    for (char c : text) {
        if (current_x + 5 >= WIDTH) {
            break;  // Stop if we run out of space
        }
        drawChar(current_x, y, c);
        current_x += 6;  // 5 pixels for character + 1 space
    }
}

void OLEDDisplay::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool on) {
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int x = x0, y = y0;
    
    while (true) {
        drawPixel(x, y, on);
        
        if (x == x1 && y == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void OLEDDisplay::drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool filled, bool on) {
    if (filled) {
        for (uint8_t i = 0; i < height; i++) {
            drawLine(x, y + i, x + width - 1, y + i, on);
        }
    } else {
        drawLine(x, y, x + width - 1, y, on);
        drawLine(x + width - 1, y, x + width - 1, y + height - 1, on);
        drawLine(x, y + height - 1, x + width - 1, y + height - 1, on);
        drawLine(x, y, x, y + height - 1, on);
    }
}

void OLEDDisplay::drawCircle(uint8_t x, uint8_t y, uint8_t radius, bool filled, bool on) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int px = 0, py = radius;
    
    drawPixel(x, y + radius, on);
    drawPixel(x, y - radius, on);
    drawPixel(x + radius, y, on);
    drawPixel(x - radius, y, on);
    
    while (px < py) {
        if (f >= 0) {
            py--;
            ddF_y += 2;
            f += ddF_y;
        }
        px++;
        ddF_x += 2;
        f += ddF_x;
        
        if (filled) {
            drawLine(x - px, y + py, x + px, y + py, on);
            drawLine(x - px, y - py, x + px, y - py, on);
            drawLine(x - py, y + px, x + py, y + px, on);
            drawLine(x - py, y - px, x + py, y - px, on);
        } else {
            drawPixel(x + px, y + py, on);
            drawPixel(x - px, y + py, on);
            drawPixel(x + px, y - py, on);
            drawPixel(x - px, y - py, on);
            drawPixel(x + py, y + px, on);
            drawPixel(x - py, y + px, on);
            drawPixel(x + py, y - px, on);
            drawPixel(x - py, y - px, on);
        }
    }
}

void OLEDDisplay::update() {
    // Write buffer to display using page addressing mode
    for (uint8_t page = 0; page < PAGES; page++) {
        // Set page address
        writeCommand(SET_PAGE_ADDRESS | page);
        
        // Set column address to 0
        writeCommand(SET_COLUMN_ADDRESS_LOW | 0);
        writeCommand(SET_COLUMN_ADDRESS_HIGH | 0);
        
        // Write page data
        uint8_t page_data[WIDTH + 1];
        page_data[0] = 0x40;  // Data control byte
        
        for (uint8_t col = 0; col < WIDTH; col++) {
            page_data[col + 1] = buffer[page * WIDTH + col];
        }
        
        writeData(page_data, WIDTH + 1);
    }
}
