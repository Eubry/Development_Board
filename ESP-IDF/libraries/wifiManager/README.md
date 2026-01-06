# wifiManager Library (ESP-IDF)

ESP-IDF library providing simplified WiFi connection management with automatic reconnection, hostname configuration, and mDNS support for ESP32 development.

## Description

The `wifiManager` library provides a high-level, object-oriented interface for managing WiFi connections on ESP32 using ESP-IDF. It handles the complexity of WiFi initialization, event handling, automatic reconnection, and network configuration, allowing developers to focus on application logic rather than WiFi stack management.

## Features

### WiFi Connection Management
- **Multiple Modes**: Station (STA), Access Point (AP), or both (APSTA)
- **Automatic Reconnection**: Configurable retry logic with maximum attempts
- **Event-Driven**: Handles WiFi and IP events automatically
- **Connection Status**: Real-time connection monitoring

### Network Configuration
- **Custom Hostname**: Set meaningful device names
- **DHCP Support**: Automatic IP address assignment
- **IP Address Retrieval**: Get assigned IP address programmatically
- **Network Interface Management**: Proper netif initialization and cleanup

### mDNS Integration
- **Automatic Publishing**: Device hostname advertised on local network
- **Local Domain**: Access device via `hostname.local`
- **Service Discovery**: Compatible with Bonjour/Avahi

### Security
- **WPA2-PSK**: Secure WiFi connections
- **PMF Support**: Protected Management Frames capable
- **Open Networks**: Support for unsecured APs (AP mode)

### Error Handling
- **Retry Logic**: Automatic reconnection attempts on disconnect
- **Detailed Logging**: ESP_LOG integration for debugging
- **Graceful Degradation**: Proper error reporting and cleanup

## Installation

### ESP-IDF Project

1. Copy the wifiManager folder to your project's components directory:
   ```bash
   cp -r ESP-IDF/libraries/wifiManager your-project/components/
   ```

2. The component requires the Counter library:
   ```bash
   cp -r Common/libraries/Counter your-project/components/
   ```

3. The component will be automatically detected by ESP-IDF build system

4. Include in your code:
   ```cpp
   #include "wifiManager.h"
   ```

## Usage Examples

### Basic WiFi Connection (Station Mode)

```cpp
#include "wifiManager.h"

extern "C" void app_main(void)
{
    // Create WiFi connection
    // Parameters: SSID, Password, Max Retries, Hostname
    wifiConnection wifi("MyWiFiNetwork", "MyPassword", 5, "esp32-device");
    
    // Connect in Station mode
    esp_err_t result = wifi.begin(WIFI_MODE_STA);
    
    if (result == ESP_OK) {
        ESP_LOGI("MAIN", "Connected to WiFi!");
        ESP_LOGI("MAIN", "IP: %s", wifi.getIp().c_str());
        ESP_LOGI("MAIN", "Hostname: %s", wifi.getHostname().c_str());
    } else {
        ESP_LOGE("MAIN", "Failed to connect");
    }
    
    // Your application code here
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Connection Status Monitoring

```cpp
#include "wifiManager.h"

extern "C" void app_main(void)
{
    wifiConnection wifi("MyWiFi", "MyPass", 5, "esp32-monitor");
    
    if (wifi.begin(WIFI_MODE_STA) == ESP_OK) {
        ESP_LOGI("MAIN", "Initial connection successful");
        
        while(1) {
            if (wifi.isConnected()) {
                ESP_LOGI("MAIN", "WiFi connected - IP: %s", wifi.getIp().c_str());
            } else {
                ESP_LOGW("MAIN", "WiFi disconnected - reconnecting...");
            }
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}
```

### Access Point Mode

```cpp
#include "wifiManager.h"

extern "C" void app_main(void)
{
    // Create WiFi Access Point
    wifiConnection ap("ESP32-Hotspot", "password123", 0, "esp32-ap");
    
    // Start in AP mode
    esp_err_t result = ap.begin(WIFI_MODE_AP);
    
    if (result == ESP_OK) {
        ESP_LOGI("MAIN", "Access Point started");
        ESP_LOGI("MAIN", "SSID: ESP32-Hotspot");
        ESP_LOGI("MAIN", "Password: password123");
        ESP_LOGI("MAIN", "Max connections: 4");
    }
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Station + Access Point Mode (APSTA)

```cpp
#include "wifiManager.h"

extern "C" void app_main(void)
{
    // Connect to WiFi and create AP simultaneously
    wifiConnection wifi("HomeNetwork", "homepass", 5, "esp32-dual");
    
    esp_err_t result = wifi.begin(WIFI_MODE_APSTA);
    
    if (result == ESP_OK) {
        ESP_LOGI("MAIN", "Dual mode active");
        ESP_LOGI("MAIN", "Station IP: %s", wifi.getIp().c_str());
        ESP_LOGI("MAIN", "AP SSID: HomeNetwork");
    }
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Custom Retry Configuration

```cpp
#include "wifiManager.h"

extern "C" void app_main(void)
{
    // 10 retry attempts before giving up
    wifiConnection wifi("MyNetwork", "mypass", 10, "persistent-esp32");
    
    esp_err_t result = wifi.begin(WIFI_MODE_STA);
    
    if (result == ESP_OK) {
        ESP_LOGI("MAIN", "Connected successfully");
    } else {
        ESP_LOGE("MAIN", "Failed after 10 retries");
        // Handle failure (fallback to AP mode, etc.)
    }
}
```

### Graceful Shutdown

```cpp
#include "wifiManager.h"

void shutdownNetwork(wifiConnection& wifi) {
    ESP_LOGI("MAIN", "Shutting down WiFi...");
    
    esp_err_t result = wifi.stop();
    
    if (result == ESP_OK) {
        ESP_LOGI("MAIN", "WiFi stopped successfully");
    }
}

extern "C" void app_main(void)
{
    wifiConnection wifi("MyWiFi", "MyPass", 5, "esp32-shutdown");
    wifi.begin(WIFI_MODE_STA);
    
    // Do work...
    vTaskDelay(pdMS_TO_TICKS(60000));
    
    // Clean shutdown
    shutdownNetwork(wifi);
}
```

## API Reference

### wifiConnection Constructor

```cpp
wifiConnection(std::string ssid, std::string password, uint8_t maxRetries = 5, std::string hostname = "esp32-device")
```

Create a WiFi connection instance.

**Parameters:**
- `ssid` - WiFi network name (for STA mode) or AP name (for AP mode)
- `password` - WiFi password (WPA2-PSK for STA, empty string for open AP)
- `maxRetries` - Maximum reconnection attempts on disconnect (default: 5)
- `hostname` - Device hostname for mDNS and network identification (default: "esp32-device")

**Behavior:**
- Stores connection parameters
- Creates FreeRTOS event group for connection status
- Does not initialize WiFi until `begin()` is called

**Example:**
```cpp
wifiConnection wifi("MyNetwork", "SecurePassword", 10, "living-room-sensor");
```

### begin()

```cpp
esp_err_t begin(wifi_mode_t mode = WIFI_MODE_STA)
```

Initialize and start WiFi connection.

**Parameters:**
- `mode` - WiFi operating mode (default: WIFI_MODE_STA)
  - `WIFI_MODE_STA` - Station mode (connect to existing WiFi)
  - `WIFI_MODE_AP` - Access Point mode (create WiFi hotspot)
  - `WIFI_MODE_APSTA` - Both Station and AP simultaneously

**Returns:**
- `ESP_OK` - Successfully connected (STA mode) or started (AP mode)
- `ESP_FAIL` - Connection failed after max retries
- `ESP_ERR_INVALID_ARG` - Invalid WiFi mode
- `ESP_ERR_INVALID_STATE` - Unexpected event during connection

**Behavior:**
- Initializes NVS (Non-Volatile Storage)
- Creates network interface (netif)
- Sets custom hostname
- Configures WiFi with provided credentials
- Registers event handlers
- Starts WiFi and waits for connection (STA mode)
- Initializes mDNS for hostname publishing

**Blocking:** In STA mode, blocks until connected or all retries exhausted

**Example:**
```cpp
esp_err_t result = wifi.begin(WIFI_MODE_STA);
if (result == ESP_OK) {
    ESP_LOGI("APP", "Connected!");
}
```

### isConnected()

```cpp
bool isConnected()
```

Check current WiFi connection status.

**Returns:**
- `true` - WiFi is connected (has IP address)
- `false` - WiFi is disconnected

**Behavior:**
- Checks event group bits for connection status
- Non-blocking
- Safe to call from any task

**Example:**
```cpp
if (wifi.isConnected()) {
    // Perform network operations
    sendData();
} else {
    // Wait for reconnection
    ESP_LOGW("APP", "Waiting for WiFi...");
}
```

### getIp()

```cpp
std::string getIp()
```

Get the current IP address assigned to the device.

**Returns:**
- IP address as string (e.g., "192.168.1.100")
- Empty string if not connected or error occurred

**Behavior:**
- Retrieves IP from STA network interface
- Logs error if netif handle not available

**Example:**
```cpp
std::string ip = wifi.getIp();
if (!ip.empty()) {
    ESP_LOGI("APP", "Device IP: %s", ip.c_str());
}
```

### getHostname()

```cpp
std::string getHostname()
```

Get the configured hostname.

**Returns:**
- Hostname as string (e.g., "esp32-device")
- Empty string if not connected or error occurred

**Behavior:**
- Retrieves hostname from STA network interface
- Logs error if netif handle not available or hostname not set

**Example:**
```cpp
std::string hostname = wifi.getHostname();
ESP_LOGI("APP", "mDNS: %s.local", hostname.c_str());
```

### stop()

```cpp
esp_err_t stop()
```

Stop WiFi and cleanup resources.

**Returns:**
- `ESP_OK` - Successfully stopped

**Behavior:**
- Stops WiFi radio
- Unregisters event handlers
- Deletes event group
- Cleans up mDNS
- Does not deinitialize NVS or netif (persistent)

**Example:**
```cpp
wifi.stop();
ESP_LOGI("APP", "WiFi shutdown complete");
```

### Destructor

```cpp
~wifiConnection()
```

Automatically called when object goes out of scope. Currently empty but could be extended for automatic cleanup.

## Event Handling

The library automatically handles these WiFi events:

### WiFi Events

| Event | Action |
|-------|--------|
| `WIFI_EVENT_STA_START` | Initiates connection to AP |
| `WIFI_EVENT_STA_DISCONNECTED` | Attempts reconnection (up to max retries) |
| `WIFI_EVENT_AP_START` | Logs AP start confirmation |

### IP Events

| Event | Action |
|-------|--------|
| `IP_EVENT_STA_GOT_IP` | Sets connected bit, initializes mDNS, logs IP |

## Configuration Details

### Station Mode (STA) Configuration

```cpp
wifi_config_t wifiConfig = {};
wifiConfig.sta.ssid = "YourSSID";
wifiConfig.sta.password = "YourPassword";
wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;  // WPA2 required
wifiConfig.sta.pmf_cfg.capable = true;   // PMF capable
wifiConfig.sta.pmf_cfg.required = false; // PMF not required
```

### Access Point (AP) Configuration

```cpp
wifi_config_t wifiConfig = {};
wifiConfig.ap.ssid = "ESP32-Hotspot";
wifiConfig.ap.password = "password123";
wifiConfig.ap.ssid_len = 13;
wifiConfig.ap.max_connection = 4;        // Max 4 simultaneous clients
wifiConfig.ap.authmode = WIFI_AUTH_WPA2_PSK;
// If password empty: WIFI_AUTH_OPEN
```

## mDNS Features

When WiFi connects, the library automatically:
1. Initializes mDNS service
2. Sets hostname (e.g., "esp32-device")
3. Sets instance name (same as hostname)
4. Publishes on `.local` domain

**Access device via:**
- `http://esp32-device.local` (if running web server)
- `ping esp32-device.local`
- SSH, Telnet, or any network service using hostname

**Test mDNS resolution:**
```bash
# Linux/Mac
avahi-browse -a          # List all mDNS services
ping esp32-device.local

# Windows (requires Bonjour)
ping esp32-device.local
dns-sd -B _services._dns-sd._udp
```

## Connection Flow

### Station Mode Sequence

1. **Initialization**
   - NVS flash initialized
   - Network interface created
   - WiFi stack initialized

2. **Configuration**
   - Hostname set on netif
   - WiFi mode set (STA)
   - SSID and password configured
   - Event handlers registered

3. **Connection**
   - WiFi started
   - `WIFI_EVENT_STA_START` → `esp_wifi_connect()` called
   - Association with AP attempted

4. **Success Path**
   - `IP_EVENT_STA_GOT_IP` received
   - mDNS initialized and hostname published
   - Connected bit set in event group
   - `begin()` returns `ESP_OK`

5. **Failure Path**
   - `WIFI_EVENT_STA_DISCONNECTED` received
   - Retry counter incremented
   - If retries < max: reconnect attempt
   - If retries >= max: fail bit set, `begin()` returns `ESP_FAIL`

## Troubleshooting

### Cannot Connect to WiFi

**Symptoms:** `begin()` returns `ESP_FAIL`, logs show "Failed after X retries"

**Solutions:**
- Verify SSID and password are correct
- Check WiFi signal strength (move closer to router)
- Ensure router is 2.4 GHz (ESP32 doesn't support 5 GHz)
- Check router authentication mode (WPA2-PSK recommended)
- Increase max retries in constructor
- Check ESP_LOG output for specific disconnect reason

### mDNS Not Resolving

**Symptoms:** Cannot access device via `hostname.local`

**Solutions:**
- **Windows**: Install Bonjour service (iTunes includes it, or standalone)
- **Linux**: Ensure Avahi daemon running: `sudo systemctl start avahi-daemon`
- **Mac**: Should work out of the box
- Verify device is connected: `wifi.isConnected()`
- Check firewall settings (mDNS uses UDP port 5353)
- Try `avahi-browse -a` (Linux) or `dns-sd -B _services._dns-sd._udp` (Mac/Win)

### Connection Drops Frequently

**Symptoms:** `isConnected()` returns false periodically

**Solutions:**
- Check WiFi signal strength
- Verify power supply is stable
- Disable power-saving mode if enabled
- Increase max retries for faster reconnection
- Check router logs for AP-initiated disconnects
- Monitor serial output for disconnect reasons

### IP Address Not Retrieved

**Symptoms:** `getIp()` returns empty string

**Solutions:**
- Wait a few seconds after `begin()` returns
- Check DHCP is enabled on router
- Verify connection with `isConnected()`
- Check serial logs for `IP_EVENT_STA_GOT_IP`
- Try static IP configuration if DHCP fails

### Hostname Not Working

**Symptoms:** Hostname empty or default value

**Solutions:**
- Ensure hostname set before calling `begin()`
- Check mDNS initialization succeeded (check logs)
- Verify hostname follows mDNS rules (no spaces, special chars)
- Try shorter hostname (< 63 characters)

## Advanced Usage

### Static IP Configuration

```cpp
// After creating netif but before wifi.begin()
esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

esp_netif_dhcpc_stop(netif);  // Stop DHCP client

esp_netif_ip_info_t ip_info;
IP4_ADDR(&ip_info.ip, 192, 168, 1, 100);
IP4_ADDR(&ip_info.gw, 192, 168, 1, 1);
IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);

esp_netif_set_ip_info(netif, &ip_info);
```

### Custom DNS Servers

```cpp
esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

esp_netif_dns_info_t dns;
IP4_ADDR(&dns.ip.u_addr.ip4, 8, 8, 8, 8);  // Google DNS
esp_netif_set_dns_info(netif, ESP_NETIF_DNS_MAIN, &dns);
```

### WiFi Power Saving

```cpp
// After wifi.begin()
esp_wifi_set_ps(WIFI_PS_MIN_MODEM);  // Minimum power save
// or
esp_wifi_set_ps(WIFI_PS_MAX_MODEM);  // Maximum power save
```

## Dependencies

- ESP-IDF (v4.4 or later)
- Counter library (dependency in header, minimal usage)
- Components:
  - `esp_wifi` - WiFi stack
  - `esp_netif` - Network interface
  - `nvs_flash` - Non-volatile storage
  - `mdns` - mDNS service
  - `esp_event` - Event loop
  - `lwip` - TCP/IP stack

## Examples

See the [WiFiManagerExample](../../examples/WiFiManagerExample) for a comprehensive demonstration.

## Known Limitations

- **5 GHz Not Supported**: ESP32 only supports 2.4 GHz WiFi
- **Single STA Connection**: Can only connect to one AP at a time
- **Max 4 AP Clients**: In AP mode, maximum 4 simultaneous connections
- **WPA3 Support**: Limited (hardware dependent)
- **Enterprise WiFi**: Requires additional configuration not covered by this library

## Future Enhancements

Potential additions:
- WPS (WiFi Protected Setup) support
- WiFi provisioning (BLE/SoftAP)
- WiFi scanning and AP selection
- Signal strength monitoring (RSSI)
- Connection quality metrics
- Multiple WiFi credential storage

## License

Open source - free to use and modify.
