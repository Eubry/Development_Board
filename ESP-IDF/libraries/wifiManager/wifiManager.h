#pragma once
// #include "freertos/FreeRTOS.h"
// #include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp_wifi.h"        // ESP-IDF WiFi
#include "esp_netif.h"       // Network interface
#include "lwip/sockets.h"    // For socket-based clients

#include "esp_event.h"
#include "nvs_flash.h"
#include "Counter.h"
#include <map>
#include <string>

//--------WiFi Connection Class--------
extern wifi_init_config_t wconfig;
extern wifi_mode_t currentWiFiMode;
//-------------------------------------
class wifiConnection{
    private:
        std::string _ssid;
        std::string _password;
        std::string _hostname;
        uint8_t _maxRetries;
        uint8_t _retryCount;
        wifi_mode_t _wifiMode;
        EventGroupHandle_t _wifiEventGroup;
        static constexpr int WIFI_CONNECTED_BIT = BIT0;
        static constexpr int WIFI_FAIL_BIT      = BIT1;
        static void wifiEventHandler(void* arg, esp_event_base_t event_base,
                                     int32_t event_id, void* event_data);
    public:
        wifiConnection(std::string ssid, std::string password, uint8_t maxRetries=5, std::string hostname="esp32-device");
        esp_err_t begin(wifi_mode_t mode=WIFI_MODE_STA);
        bool isConnected();
        std::string getIp();
        std::string getHostname();
        esp_err_t stop();
        ~wifiConnection();
};