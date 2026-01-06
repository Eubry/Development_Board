#include "wifiManager.h"

// Define global variables
wifi_init_config_t wconfig;
wifi_mode_t currentWiFiMode = WIFI_MODE_NULL;

//-------------------------------------
wifiConnection::wifiConnection(std::string ssid, std::string password, uint8_t maxRetries)
: _ssid(ssid), _password(password), _maxRetries(maxRetries), _retryCount(0), _wifiMode(WIFI_MODE_NULL) {
    _wifiEventGroup = xEventGroupCreate();
}
esp_err_t wifiConnection::begin(wifi_mode_t mode){
    _wifiMode = mode;
    esp_err_t ret;

    // Initialize NVS
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the underlying TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create default WiFi station or AP
    if (_wifiMode == WIFI_MODE_STA) {
        esp_netif_create_default_wifi_sta();
    } else if (_wifiMode == WIFI_MODE_AP) {
        esp_netif_create_default_wifi_ap();
    } else if (_wifiMode == WIFI_MODE_APSTA) {
        esp_netif_create_default_wifi_ap();
        esp_netif_create_default_wifi_sta();
    } else {
        ESP_LOGE("wConnection", "Invalid WiFi mode");
        return ESP_ERR_INVALID_ARG;
    }

    // Initialize WiFi with default configuration
    wconfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wconfig));

    // Register event handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifiConnection::wifiEventHandler,
                                                        this,
                                                        nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifiConnection::wifiEventHandler,
                                                        this,
                                                        nullptr));

    // Set WiFi mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(_wifiMode));

    // Configure WiFi connection settings
    wifi_config_t wifiConfig = {};
    if (_wifiMode == WIFI_MODE_STA || _wifiMode == WIFI_MODE_APSTA) {
        strncpy((char*)wifiConfig.sta.ssid, _ssid.c_str(), sizeof(wifiConfig.sta.ssid));
        strncpy((char*)wifiConfig.sta.password, _password.c_str(), sizeof(wifiConfig.sta.password));
        wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        wifiConfig.sta.pmf_cfg.capable = true;
        wifiConfig.sta.pmf_cfg.required = false;
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    } else if (_wifiMode == WIFI_MODE_AP) {
        strncpy((char*)wifiConfig.ap.ssid, _ssid.c_str(), sizeof(wifiConfig.ap.ssid));
        strncpy((char*)wifiConfig.ap.password, _password.c_str(), sizeof(wifiConfig.ap.password));
        wifiConfig.ap.ssid_len = _ssid.length();
        wifiConfig.ap.max_connection = 4;
        wifiConfig.ap.authmode = WIFI_AUTH_WPA2_PSK;
        if (strlen(_password.c_str()) == 0) {
            wifiConfig.ap.authmode = WIFI_AUTH_OPEN;
        }
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifiConfig));
    } else {
        ESP_LOGE("wConnection", "Invalid WiFi mode for configuration");
        return ESP_ERR_INVALID_ARG;
    }
    // Start WiFi
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI("wConnection", "WiFi initialization completed. Attempting to connect to SSID: %s", _ssid.c_str());
    // Wait for connection or failure
    EventBits_t bits = xEventGroupWaitBits(_wifiEventGroup,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI("wConnection", "Connected to WiFi SSID:%s", _ssid.c_str());
        return ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI("wConnection", "Failed to connect to SSID:%s", _ssid.c_str());
        return ESP_FAIL;
    } else {
        ESP_LOGE("wConnection", "Unexpected event");
        return ESP_ERR_INVALID_STATE;
    }
}
void wifiConnection::wifiEventHandler(void* arg, esp_event_base_t event_base,
                                  int32_t event_id, void* event_data) {
    wifiConnection* instance = static_cast<wifiConnection*>(arg);
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_DISCONNECTED: {
                wifi_event_sta_disconnected_t* disconnected = static_cast<wifi_event_sta_disconnected_t*>(event_data);
                ESP_LOGW("wConnection", "Disconnected from WiFi. Reason: %d", disconnected->reason);
                if (instance->_retryCount < instance->_maxRetries) {
                    esp_wifi_connect();
                    instance->_retryCount++;
                    ESP_LOGI("wConnection", "Retry %d/%d to connect to the AP", instance->_retryCount, instance->_maxRetries);
                } else {
                    ESP_LOGE("wConnection", "Failed after %d retries", instance->_maxRetries);
                    xEventGroupSetBits(instance->_wifiEventGroup, WIFI_FAIL_BIT);
                }
                break;
            }
            case WIFI_EVENT_AP_START:
                ESP_LOGI("wConnection", "WiFi AP started");
                break;
            default:
                break;
        }
    } else if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP: {
                ip_event_got_ip_t* event = static_cast<ip_event_got_ip_t*>(event_data);
                ESP_LOGI("wConnection", "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
                instance->_retryCount = 0;
                xEventGroupSetBits(instance->_wifiEventGroup, WIFI_CONNECTED_BIT);
                break;
            }
            default:
                break;
        }
    }
}
bool wifiConnection::isConnected() {
    EventBits_t bits = xEventGroupGetBits(_wifiEventGroup);
    return (bits & WIFI_CONNECTED_BIT) != 0;
}
esp_err_t wifiConnection::stop() {
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, nullptr));
    vEventGroupDelete(_wifiEventGroup);
    ESP_LOGI("wifiConnection", "WiFi stopped.");
    return ESP_OK;
}
std::string wifiConnection::getIp() {
    esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif == nullptr) {
        ESP_LOGE("wifiConnection", "Failed to get netif handle");
        return "";
    }
    esp_netif_ip_info_t ipInfo;
    if (esp_netif_get_ip_info(netif, &ipInfo) != ESP_OK) {
        ESP_LOGE("wifiConnection", "Failed to get IP info");
        return "";
    }
    char ipStr[16];
    snprintf(ipStr, sizeof(ipStr), IPSTR, IP2STR(&ipInfo.ip));
    return std::string(ipStr);
}
wifiConnection::~wifiConnection() {
    stop();
}
//-------------------------------------