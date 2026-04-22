idf_component_register(
    SRCS "motorMgr.cpp"
    INCLUDE_DIRS "."
    REQUIRES freertos esp_system esp_timer pinManager
)
