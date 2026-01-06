idf_component_register(
    SRCS "wifiManager.cpp"
    INCLUDE_DIRS "."
    REQUIRES Counter esp_wifi esp_netif
    PRIV_REQUIRES nvs_flash esp_netif_stack esp_event
)
