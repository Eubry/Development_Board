idf_component_register(
    SRCS "webManager.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_http_server esp_timer
)
