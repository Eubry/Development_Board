# WiFiManager Example

This example demonstrates the capabilities of the `wifiManager` library for ESP-IDF, showcasing WiFi connection management including Station (STA) mode with automatic reconnection, hostname configuration, and mDNS support.

## What This Example Does

The example demonstrates:
- Connecting to WiFi in Station (STA) mode
- Setting a custom hostname for the device
- Automatic reconnection with retry logic
- Getting IP address and hostname information
- mDNS hostname publishing for local network discovery
- WiFi connection status monitoring
- Graceful WiFi shutdown

## Features Demonstrated

1. **WiFi Station Mode**: Connect ESP32 to an existing WiFi network
2. **Custom Hostname**: Set a meaningful device name (e.g., "esp32-demo")
3. **Retry Logic**: Automatic reconnection attempts with configurable retry limit
4. **mDNS Support**: Access device via `hostname.local` on local network
5. **Connection Monitoring**: Check connection status and retrieve network info
6. **Error Handling**: Proper event handling for connection success/failure

## Configuration

Before building, you need to configure your WiFi credentials:

1. Open [main.cpp](main/main.cpp)
2. Modify these constants:
   ```cpp
   #define WIFI_SSID "YourWiFiSSID"
   #define WIFI_PASSWORD "YourWiFiPassword"
   #define WIFI_HOSTNAME "esp32-demo"
   ```

## How to Build and Flash

1. Navigate to this example directory:
   ```bash
   cd ESP-IDF/examples/WiFiManagerExample
   ```

2. Configure the project (optional - set WiFi credentials):
   ```bash
   idf.py menuconfig
   ```
   Or edit `main.cpp` directly with your WiFi credentials.

3. Build the project:
   ```bash
   idf.py build
   ```

4. Flash to your ESP32:
   ```bash
   idf.py flash
   ```

5. Monitor the output:
   ```bash
   idf.py monitor
   ```

## Example Output

```
I (xxx) WiFiExample: === WiFi Manager Example ===
I (xxx) WiFiExample: ESP-IDF Version: v5.x.x
I (xxx) WiFiExample: Connecting to WiFi...
I (xxx) wConnection: Hostname set to: esp32-demo
I (xxx) wConnection: WiFi initialization completed. Attempting to connect to SSID: YourWiFi
I (xxx) wConnection: Got IP:192.168.1.100
I (xxx) wConnection: mDNS hostname published: esp32-demo.local
I (xxx) wConnection: Connected to WiFi SSID:YourWiFi
I (xxx) WiFiExample: Successfully connected to WiFi!
I (xxx) WiFiExample: IP Address: 192.168.1.100
I (xxx) WiFiExample: Hostname: esp32-demo
I (xxx) WiFiExample: You can access this device at: esp32-demo.local
I (xxx) WiFiExample: Connection status: Connected
I (xxx) WiFiExample: WiFi is connected. Device is operational.
I (xxx) WiFiExample: Monitoring connection... (Ctrl+] to exit)
```

## Network Access

Once connected, you can access your ESP32 via:
- **IP Address**: `192.168.1.x` (shown in serial output)
- **mDNS Hostname**: `esp32-demo.local` (or your custom hostname)

Test mDNS resolution:
```bash
# On Linux/Mac
ping esp32-demo.local

# On Windows (requires Bonjour service)
ping esp32-demo.local
```

## Key Concepts

### WiFi Station Mode
- Device acts as a WiFi client
- Connects to existing access point (router)
- Receives IP address via DHCP
- Can communicate with other network devices

### Hostname Configuration
- Custom hostname makes device easily identifiable
- Used for mDNS resolution (`hostname.local`)
- Set during WiFi initialization

### Retry Logic
- Configurable maximum retry attempts (default: 5)
- Automatic reconnection on disconnect
- Event-driven reconnection handling

### mDNS (Multicast DNS)
- Enables `.local` domain resolution
- No need to remember IP addresses
- Works on local network without DNS server

### Event Handling
The library handles these WiFi events:
- `WIFI_EVENT_STA_START`: WiFi started
- `WIFI_EVENT_STA_DISCONNECTED`: Connection lost
- `IP_EVENT_STA_GOT_IP`: IP address assigned

## Troubleshooting

### Cannot Connect to WiFi
- Verify SSID and password are correct
- Check WiFi signal strength
- Ensure router is 2.4 GHz (ESP32 doesn't support 5 GHz)
- Check authentication mode matches (WPA2-PSK recommended)

### mDNS Not Resolving
- Ensure device is on same network
- Windows requires Bonjour service (iTunes or standalone)
- Linux/Mac should work out of the box
- Try `avahi-browse -a` (Linux) to see advertised services

### Connection Drops
- Check WiFi signal strength
- Increase max retries in code
- Check router stability
- Consider implementing WiFi power-saving modes

### IP Address Not Showing
- Wait for IP assignment (may take a few seconds)
- Check DHCP server is enabled on router
- Verify network cable/antenna connection

## Advanced Usage

### Changing WiFi Mode
The library supports multiple WiFi modes:
- `WIFI_MODE_STA`: Station mode (client)
- `WIFI_MODE_AP`: Access Point mode (create hotspot)
- `WIFI_MODE_APSTA`: Both STA and AP simultaneously

Modify the `begin()` call:
```cpp
wifi.begin(WIFI_MODE_AP);  // Run as access point
```

### Adjusting Retry Count
Modify the constructor:
```cpp
wifiConnection wifi(WIFI_SSID, WIFI_PASSWORD, 10, WIFI_HOSTNAME);  // 10 retries
```

### Monitoring Connection Status
```cpp
while (1) {
    if (wifi.isConnected()) {
        ESP_LOGI(TAG, "IP: %s", wifi.getIp().c_str());
    } else {
        ESP_LOGI(TAG, "Not connected");
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
}
```

## See Also

- [wifiManager Library Documentation](../../libraries/wifiManager/README.md)
- [ESP-IDF WiFi API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)
- [ESP-IDF mDNS Service](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mdns.html)
- [ESP-IDF Network Interface](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_netif.html)
