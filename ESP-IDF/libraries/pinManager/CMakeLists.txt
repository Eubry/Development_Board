idf_component_register(
    SRCS "pinManager.cpp"
    INCLUDE_DIRS "."
    REQUIRES Counter esp_driver_gpio esp_driver_ledc
)
