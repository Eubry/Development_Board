idf_component_register(
    SRCS "oledDisplay.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_i2c esp_common
)
