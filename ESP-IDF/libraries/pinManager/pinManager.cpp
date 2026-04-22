#include "pinManager.h"

#define PIN_TAG "PinManager"

void pinManager::digitalPin(std::string name, int8_t pin, gpio_mode_t mode){
    // Validate pin number for ESP32 (GPIO 0-39, with some reserved pins)
    if(pin < 0 || pin > 39) {
        ESP_LOGE(PIN_TAG, "Invalid GPIO pin number: %d. ESP32 supports GPIO 0-39 only.", pin);
        return;
    }
    
    pinMap[name]={static_cast<gpio_num_t>(pin), mode, PinType::DIGITAL, {0, 0, 0}};
    // Configure GPIO 32 as input
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = mode;
    io_conf.pin_bit_mask = (1ULL << pin);
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}
int pinManager::digitalRead(std::string name){
    if(pinMap.find(name) != pinMap.end()){
        if(pinMap[name].type == PinType::DIGITAL && (pinMap[name].mode == GPIO_MODE_INPUT || pinMap[name].mode == GPIO_MODE_INPUT_OUTPUT)){
            gpio_num_t pin = pinMap[name].pin;
            return gpio_get_level(pin);
        }else{return -1;}
    }else{return -1;}
}
void pinManager::digitalWrite(std::string name, uint8_t value){
    if(pinMap.find(name) != pinMap.end()){
        if(pinMap[name].type == PinType::DIGITAL && (pinMap[name].mode == GPIO_MODE_OUTPUT || pinMap[name].mode == GPIO_MODE_INPUT_OUTPUT)){
            gpio_num_t pin = pinMap[name].pin;
            gpio_set_level(pin, value);
        }
    }
}

void pinManager::pwmPin(std::string name, int8_t pin, uint32_t frequency, ledc_timer_t timer, ledc_timer_bit_t duty_resolution){
    // Validate pin number for ESP32 (GPIO 0-39, with some reserved pins)
    if(pin < 0 || pin > 39) {
        ESP_LOGE(PIN_TAG, "Invalid GPIO pin number: %d. ESP32 supports GPIO 0-39 only.", pin);
        return;
    }
    
    // Assign next available channel
    ledc_channel_t channel = static_cast<ledc_channel_t>(nextChannel);
    nextChannel++;
    
    // Configure timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = duty_resolution,
        .timer_num = timer,
        .freq_hz = frequency,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };
    ledc_timer_config(&ledc_timer);
    
    // Configure channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = timer,
        .duty = 0,
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {},
        .deconfigure = false
    };
    ledc_channel_config(&ledc_channel);
    
    // Store PWM info
    pwmMap[name] = {static_cast<gpio_num_t>(pin), channel, timer, frequency, cTime(), 0, false};
}
// Set absolute duty (0-8191)
void pinManager::setPwmDuty(std::string name, uint32_t duty){
    if(pwmMap.find(name) != pwmMap.end()){
        ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel);
    }
}
// Set duty cycle by percentage (0-100%)
void pinManager::setPwmDutyPercent(std::string name, int8_t percent){setPwmDutyPercent(name, (float) percent);}
void pinManager::setPwmDutyPercent(std::string name, float percent){
    if(pwmMap.find(name) != pwmMap.end()){
        // 13-bit resolution = 8192 levels (0-8191)
        uint32_t duty = (uint32_t)((percent / 100.0f) * 8191.0f);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel);
    }
}
// Set duty cycle by microseconds (for servo control)
void pinManager::setPwmDutyMicros(std::string name, uint32_t micros){
    if(pwmMap.find(name) != pwmMap.end()){
        // Calculate duty cycle from microseconds
        // duty = (micros * frequency * max_duty) / 1000000
        uint32_t frequency = pwmMap[name].frequency;
        uint32_t duty = (micros * frequency * 8192) / 1000000;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel);
    }
}
// Set PWM frequency - example usage: pin.setPwmFrequency("led", 1000); // Change frequency to 1 kHz
void pinManager::setPwmFrequency(std::string name, uint32_t frequency){
    if(pwmMap.find(name) != pwmMap.end()){
        ledc_set_freq(LEDC_LOW_SPEED_MODE, pwmMap[name].timer, frequency);
        pwmMap[name].frequency = frequency;
    }
}
// Generate tone at specific frequency with adjustable volume (0-100%) and optional duration
void pinManager::tone(std::string name, uint32_t frequency, uint8_t volume, uint32_t duration_ms){
    if(pwmMap.find(name) != pwmMap.end()){
        ledc_set_freq(LEDC_LOW_SPEED_MODE, pwmMap[name].timer, frequency);
        pwmMap[name].frequency = frequency;
        // Volume controls duty cycle: 0% = silent, 50% = default, 100% = loudest
        // Clamp volume to 0-100 range
        if(volume > 100) volume = 100;
        uint32_t duty = (volume * 8192) / 100; // Convert percentage to 13-bit duty value
        ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel);
        
        // If duration specified, start non-blocking timer
        if(duration_ms > 0){
            pwmMap[name].duration_us = duration_ms * 1000; // Convert ms to microseconds
            pwmMap[name].toneTimer.reset();
            pwmMap[name].toneActive = true;
        } else {
            pwmMap[name].toneActive = false;
        }
    }
}
// Stop tone
void pinManager::noTone(std::string name){
    if(pwmMap.find(name) != pwmMap.end()){
        ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel);
        pwmMap[name].toneActive = false;
    }
}
// Update method - call this regularly in your main loop to handle timed tones
void pinManager::update(){
    for(auto& pwm : pwmMap){
        if(pwm.second.toneActive){
            pwm.second.toneTimer.wait(pwm.second.duration_us);
            if(pwm.second.toneTimer.finish()){
                noTone(pwm.first);
            }
        }
    }
}
// Register an ADC pin (GPIO 1-10, ADC1 channels 0-9)
void pinManager::analogPin(std::string name, int8_t pin){
    if(pin < 1 || pin > 10){
        ESP_LOGE(PIN_TAG, "Invalid ADC pin: %d. ESP32-S3 ADC1 supports GPIO 1-10 only.", pin);
        return;
    }
    if(adcUnit == nullptr){
        adc_oneshot_unit_init_cfg_t unit_cfg = {
            .unit_id = ADC_UNIT_1,
            .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
            .ulp_mode = ADC_ULP_MODE_DISABLE
        };
        adc_oneshot_new_unit(&unit_cfg, &adcUnit);
    }
    adc_channel_t channel = static_cast<adc_channel_t>(pin - 1);
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_oneshot_config_channel(adcUnit, channel, &chan_cfg);
    adcMap[name] = {static_cast<gpio_num_t>(pin), channel};
}
// Read raw ADC value (0-4095)
int pinManager::analogRead(std::string name){
    if(adcMap.find(name) == adcMap.end()){
        ESP_LOGE(PIN_TAG, "ADC pin '%s' not registered. Call analogPin() first.", name.c_str());
        return -1;
    }
    int raw = 0;
    adc_oneshot_read(adcUnit, adcMap[name].channel, &raw);
    return raw;
}
// Set PWM duty cycle using Arduino-style 0-255 value
void pinManager::analogWrite(std::string name, uint8_t value){
    if(pwmMap.find(name) == pwmMap.end()){
        ESP_LOGE(PIN_TAG, "PWM pin '%s' not registered. Call pwmPin() first.", name.c_str());
        return;
    }
    uint32_t duty = ((uint32_t)value * 8191) / 255;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel);
}
/*
pin.analogPin("sensor", 4);          // register GPIO4 as ADC input
int raw = pin.analogRead("sensor");  // 0-4095

pin.pwmPin("led", 5);               // register GPIO5 as PWM output
pin.analogWrite("led", 128);         // ~50% duty (Arduino-style 0-255)
*/
// Register an ADC pin (GPIO 1-10, ADC1 channels 0-9)
/*void pinManager::analogPin(std::string name, int8_t pin){
    if(pin < 1 || pin > 10){
        ESP_LOGE(PIN_TAG, "Invalid ADC pin: %d. ESP32-S3 ADC1 supports GPIO 1-10 only.", pin);
        return;
    }
    if(adcUnit == nullptr){
        adc_oneshot_unit_init_cfg_t unit_cfg = {
            .unit_id = ADC_UNIT_1,
            .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
            .ulp_mode = ADC_ULP_MODE_DISABLE
        };
        adc_oneshot_new_unit(&unit_cfg, &adcUnit);
    }
    adc_channel_t channel = static_cast<adc_channel_t>(pin - 1);
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_oneshot_config_channel(adcUnit, channel, &chan_cfg);
    adcMap[name] = {static_cast<gpio_num_t>(pin), channel};
}
// Read raw ADC value (0-4095)
int pinManager::analogRead(std::string name){
    if(adcMap.find(name) == adcMap.end()){
        ESP_LOGE(PIN_TAG, "ADC pin '%s' not registered. Call analogPin() first.", name.c_str());
        return -1;
    }
    int raw = 0;
    adc_oneshot_read(adcUnit, adcMap[name].channel, &raw);
    return raw;
}
// Set PWM duty cycle using Arduino-style 0-255 value
void pinManager::analogWrite(std::string name, uint8_t value){
    if(pwmMap.find(name) == pwmMap.end()){
        ESP_LOGE(PIN_TAG, "PWM pin '%s' not registered. Call pwmPin() first.", name.c_str());
        return;
    }
    uint32_t duty = ((uint32_t)value * 8191) / 255;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmMap[name].channel);
}
*/