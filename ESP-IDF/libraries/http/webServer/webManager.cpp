#include "webManager.h"
#include "../../Utils/Utils.h"
#include <cstring> // for strcmp

#ifndef LOG_TAG
#define LOG_TAG "webManager"
#endif

serverManager::serverManager(){
    // Constructor implementation (if needed)
}
esp_err_t _handler(httpd_req_t *req) {
    serverManager::webData* ctx = (serverManager::webData*)req->user_ctx;
    ESP_LOGI(LOG_TAG, "_handler called. ctx: %p", ctx);
    if(ctx && ctx->html != nullptr){
        const char* resp_str = ctx->html;
        ESP_LOGI(LOG_TAG, "ctx->html: %s", resp_str ? resp_str : "<null>");
        if(resp_str != nullptr && strcmp(resp_str, "") != 0){
            ESP_LOGI(LOG_TAG, "Sending response: %s", resp_str);
            esp_err_t send_ret = httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
            ESP_LOGI(LOG_TAG, "httpd_resp_send returned: %d", send_ret);
            return ESP_OK;
        }else{
            ESP_LOGI(LOG_TAG, "Sending default empty HTML response");
            esp_err_t send_ret = httpd_resp_send(req, "<html><body><h1>Empty</h1><p>Content was not provided! :(</p></body></html>", HTTPD_RESP_USE_STRLEN);
            ESP_LOGI(LOG_TAG, "httpd_resp_send returned: %d", send_ret);
            return ESP_OK;
        }
    }else{ // Return 204 No Content - source not available
        const char* resp_errmsg = ctx ? ctx->error : nullptr;
        const char* resp_status = ctx ? ctx->status : nullptr;
        if(resp_status == nullptr || strcmp(resp_status, "") == 0){resp_status = "204 No Content";}
        if(resp_errmsg == nullptr || strcmp(resp_errmsg, "") == 0){resp_errmsg = "Content not available";}
        ESP_LOGI(LOG_TAG, "Sending 204 No Content. Status: %s, Error: %s", resp_status, resp_errmsg);
        httpd_resp_set_status(req, resp_status);
        esp_err_t send_ret = httpd_resp_send(req, NULL, 0);
        ESP_LOGI(LOG_TAG, "httpd_resp_send (NULL) returned: %d", send_ret);
        // httpd_resp_send(req, NULL, 0);
        return ESP_OK;
    }
}
esp_err_t _apihandler(httpd_req_t *req) {
    serverManager::apiData* ctx = (serverManager::apiData*)req->user_ctx;
    if(ctx && ctx!= nullptr){
        char buf[100];
        int ret, remaining = req->content_len;
        if (remaining >= sizeof(buf)) {
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ctx->error);
            return ESP_FAIL;
        }
        ret = httpd_req_recv(req, buf, remaining);
        if (ret <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                // httpd_resp_send_408(req);
                httpd_resp_send_err(req, HTTPD_408_REQ_TIMEOUT, "Request Timeout");
            }
            return ESP_FAIL;
        }
        buf[ret] = '\0';
        ESP_LOGI(LOG_TAG,"Received data: %s", buf);
        
        for(const auto& [key, option] : ctx->option){
            if (strstr(buf, option.rx) != NULL) {// Match found, process the request
                if(option.handler != nullptr){option.handler((void*)req);}// Call custom handler if provided
                serverManager::sendResp(req,option.tx,option.type);
                return ESP_OK;
            }
        }
   return ESP_OK;

        //return ctx->handler(buf, req);// Call main handler if provided
        //return ESP_OK;
    }else{ // Return 204 No Content - source not available
        const char* resp_errmsg = ctx ? ctx->error : nullptr;
        const char* resp_status = "204 No Content";
        if(resp_errmsg == nullptr || strcmp(resp_errmsg, "") == 0){resp_errmsg = "Content not available";}
        httpd_resp_set_status(req, resp_status);
        httpd_resp_send(req, NULL, 0);
        return ESP_OK;
    }
}
void serverManager::addHTMLPath(std::string key,httpd_method_t method,webData data){collWeb[key] = {method, data};}
void serverManager::addAPIPath(std::string key, apiData apiData){
    collApi[key] = apiData;
}
httpd_handle_t serverManager::begin(){
    httpd_handle_t server_instance = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    // Start the httpd server
    if (httpd_start(&server_instance, &config) == ESP_OK) {
        // Register HTML paths
        for(const auto& [key, webDta] : collWeb){
            httpd_uri_t uri_handler = {
                .uri       = webDta.data.uri,
                .method    = webDta.method,
                .handler   = _handler,
                .user_ctx  = (void*)&(collWeb[key].data)
            };
            httpd_register_uri_handler(server_instance, &uri_handler);
        }
        for(const auto& [key, apiDta] : collApi){
            std::string fullUri = "/" + std::string(apiDta.uri);
            httpd_uri_t uri_handler = {
                .uri       = fullUri.c_str(),
                .method    = apiDta.method,
                .handler   = _apihandler,
                .user_ctx  = (void*)&(collApi[key])
            };
            httpd_register_uri_handler(server_instance, &uri_handler);
        }
        return server_instance;
    }
    ESP_LOGI("ERRWEB", "Error starting HTTP server!");
    return NULL;
}
void serverManager::sendResp(httpd_req_t *req,const char* resp,const char* type){
    httpd_resp_set_type(req, type);
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
}
void serverManager::addStaticPath(std::string path, std::string filePath){
    // Implementation to add static file path
}

void serverManager::stop(){
    // Implementation to stop the server
}
/*void serverManager::_startRootFavicon(){
    httpd_register_uri_handler(server_instance, &root);
}*/
serverManager::~serverManager(){
    // Destructor implementation (if needed)
    stop();
}