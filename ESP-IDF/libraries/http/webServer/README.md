# webServer Library (ESP-IDF)

Minimal HTTP server wrapper for registering HTML and API endpoints using esp_http_server.

## Overview

`serverManager` wraps basic route registration and server startup for ESP-IDF's built-in HTTP server.

Main collections:

- `collWeb`: URI routes that return static HTML/text payloads.
- `collApi`: URI routes that parse request body and match configured options.

## Dependencies

From `CMakeLists.txt`:

- `esp_http_server`
- `esp_timer`

## Public API

```cpp
class serverManager {
public:
    struct webData {
        const char* uri = "";
        const char* html = "";
        const char* status = "";
        const char* error = "";
    };

    struct collWData {
        httpd_method_t method = HTTP_GET;
        webData data;
    };

    struct apiOption {
        const char* rx = "";
        const char* tx = "";
        const char* type = "application/json";
        void (*handler)(void* pvPar) = nullptr;
        void* parameter = nullptr;
    };

    struct apiData {
        httpd_method_t method = HTTP_POST;
        const char* error = "Content too long";
        const char* uri = "";
        std::map<std::string, apiOption> option;
    };

    httpd_handle_t begin();
    void addHTMLPath(std::string key, httpd_method_t method, webData data);
    void addAPIPath(std::string key, apiData apiData);
    void addStaticPath(std::string path, std::string filePath);
    static void sendResp(httpd_req_t* req, const char* resp, const char* type);
    void stop();
};
```

## Usage

```cpp
#include "webManager.h"

extern "C" void app_main(void) {
    serverManager web;

    serverManager::webData root = {
        .uri = "/",
        .html = "<html><body><h1>ESP32</h1></body></html>",
        .status = "200 OK",
        .error = ""
    };

    web.addHTMLPath("root", HTTP_GET, root);

    serverManager::apiData api;
    api.method = HTTP_POST;
    api.uri = "api/led";
    api.option["on"] = {
        .rx = "on",
        .tx = "{\"ok\":true,\"state\":\"on\"}",
        .type = "application/json"
    };

    web.addAPIPath("led", api);
    web.begin();
}
```

## Behavior Notes

- API handler reads request body into a fixed 100-byte buffer.
- API options are matched with substring search (`strstr`) against request body.
- HTML routes use `webData.html`; if empty, a fallback HTML response is sent.

## Current Limitations

- `begin()` starts a local `server_instance` and returns it, but does not assign it to the class member `server`.
- `stop()` and `addStaticPath()` are currently placeholders.
- API route URI is built as `"/" + uri`, while HTML route uses `webData.uri` directly.
- This component is suitable for prototypes; production projects should harden parsing, routing, and shutdown handling.

## Include

```cpp
#include "webManager.h"
```
