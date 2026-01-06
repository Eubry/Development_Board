/**
 * @file main.cpp
 * @brief Comprehensive example demonstrating the pinManager library
 * @version 1.0.0
 * @date 2026-01-06
 * @author Eubry Gomez Ramirez
 * 
 * This example demonstrates:
 * - Digital output (LED control)
 * - Digital input (button reading)
 * - PWM generation and control
 * - Servo motor control
 * - Tone generation with volume and duration
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "pinManager.h"

// GPIO pin definitions
#define LED_PIN 2       // Built-in LED on most ESP32 boards
#define BUTTON_PIN 32   // Button with internal pull-down
#define PWM_PIN 27      // PWM demonstration pin
#define SERVO_PIN 18    // Servo motor pin
#define BUZZER_PIN 25   // Buzzer/speaker pin

// Tag for logging
static const char *TAG = "PinExample";

// Create pin manager instance
pinManager pins;

/**
 * @brief Demonstrate digital output control
 */
void digitalOutputDemo() {
    ESP_LOGI(TAG, "--- Digital Output Test ---");
    
    for (int i = 0; i < 5; i++) {
        pins.digitalWrite("led", 1);
        ESP_LOGI(TAG, "LED ON");
        vTaskDelay(pdMS_TO_TICKS(500));
        
        pins.digitalWrite("led", 0);
        ESP_LOGI(TAG, "LED OFF");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief Demonstrate digital input reading
 */
void digitalInputDemo() {
    ESP_LOGI(TAG, "--- Digital Input Test ---");
    ESP_LOGI(TAG, "Press the button on GPIO%d...", BUTTON_PIN);
    
    for (int i = 0; i < 20; i++) {
        int buttonState = pins.digitalRead("button");
        if (buttonState == 1) {
            ESP_LOGI(TAG, "Button PRESSED!");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * @brief Demonstrate PWM control with different duty cycles
 */
void pwmDemo() {
    ESP_LOGI(TAG, "--- PWM Test ---");
    
    // Test different duty cycle percentages
    int percentages[] = {0, 25, 50, 75, 100};
    
    for (int i = 0; i < 5; i++) {
        pins.setPwmDutyPercent("pwm", percentages[i]);
        ESP_LOGI(TAG, "PWM at %d%%", percentages[i]);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    // Turn off
    pins.setPwmDutyPercent("pwm", 0);
}

/**
 * @brief Demonstrate servo control using microsecond pulse widths
 */
void servoDemo() {
    ESP_LOGI(TAG, "--- Servo Test ---");
    
    // Typical servo control: 1000µs = 0°, 1500µs = 90°, 2000µs = 180°
    
    // Move to 0 degrees
    pins.setPwmDutyMicros("servo", 1000);
    ESP_LOGI(TAG, "Servo to 0 degrees");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Move to 90 degrees
    pins.setPwmDutyMicros("servo", 1500);
    ESP_LOGI(TAG, "Servo to 90 degrees");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Move to 180 degrees
    pins.setPwmDutyMicros("servo", 2000);
    ESP_LOGI(TAG, "Servo to 180 degrees");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Return to center
    pins.setPwmDutyMicros("servo", 1500);
    ESP_LOGI(TAG, "Servo to center (90 degrees)");
    vTaskDelay(pdMS_TO_TICKS(1000));
}

/**
 * @brief Demonstrate tone generation with different frequencies
 */
void toneDemo() {
    ESP_LOGI(TAG, "--- Tone Test ---");
    
    // Play musical notes (C, E, G)
    ESP_LOGI(TAG, "Playing C note (262 Hz)");
    pins.tone("buzzer", 262, 50, 500);  // 262 Hz, 50% volume, 500ms duration
    vTaskDelay(pdMS_TO_TICKS(600));
    
    ESP_LOGI(TAG, "Playing E note (330 Hz)");
    pins.tone("buzzer", 330, 50, 500);
    vTaskDelay(pdMS_TO_TICKS(600));
    
    ESP_LOGI(TAG, "Playing G note (392 Hz)");
    pins.tone("buzzer", 392, 50, 500);
    vTaskDelay(pdMS_TO_TICKS(600));
    
    // Play a simple melody
    ESP_LOGI(TAG, "Playing simple melody...");
    int melody[] = {262, 294, 330, 349, 392, 440, 494, 523}; // C major scale
    
    for (int i = 0; i < 8; i++) {
        pins.tone("buzzer", melody[i], 30, 200);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
    
    // Ensure tone is off
    pins.noTone("buzzer");
    ESP_LOGI(TAG, "Tone demo complete");
}

/**
 * @brief Demonstrate non-blocking timed tones
 */
void timedToneDemo() {
    ESP_LOGI(TAG, "--- Timed Tone Test ---");
    
    // Start a 2-second tone
    ESP_LOGI(TAG, "Starting 2-second tone at 440 Hz...");
    pins.tone("buzzer", 440, 40, 2000);
    
    // The tone will automatically stop after 2 seconds
    // In the meantime, we can do other things
    for (int i = 0; i < 20; i++) {
        ESP_LOGI(TAG, "Doing other work while tone plays... (%d/20)", i + 1);
        pins.update(); // Call update() to handle tone timing
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    ESP_LOGI(TAG, "Timed tone should have stopped automatically");
}

/**
 * @brief Main application entry point
 */
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "=== Pin Manager Example ===");
    ESP_LOGI(TAG, "ESP-IDF Version: %s", esp_get_idf_version());
    
    // Setup pins
    ESP_LOGI(TAG, "Setting up pins...");
    
    // Digital pins
    pins.digitalPin("led", LED_PIN, GPIO_MODE_OUTPUT);
    pins.digitalPin("button", BUTTON_PIN, GPIO_MODE_INPUT);
    
    // PWM pins
    pins.pwmPin("pwm", PWM_PIN, 5000, LEDC_TIMER_0, LEDC_TIMER_13_BIT);
    pins.pwmPin("servo", SERVO_PIN, 50, LEDC_TIMER_1, LEDC_TIMER_13_BIT);  // 50 Hz for servo
    pins.pwmPin("buzzer", BUZZER_PIN, 1000, LEDC_TIMER_2, LEDC_TIMER_13_BIT);
    
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Run demonstrations
    digitalOutputDemo();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    digitalInputDemo();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    pwmDemo();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    servoDemo();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    toneDemo();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    timedToneDemo();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    ESP_LOGI(TAG, "=== All demonstrations complete! ===");
    ESP_LOGI(TAG, "Example will now loop the digital output demo...");
    
    // Continuous loop - blink LED
    while (1) {
        pins.digitalWrite("led", 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        pins.digitalWrite("led", 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
