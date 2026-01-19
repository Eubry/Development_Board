#include "../../Utils/Utils.h"
#include "esp_err.h"// Include ESP error codes
#include "esp_log.h"// Add ESP logging support
//#include "esp_timer.h"// Include ESP timer support
#include "esp_http_server.h" // Include ESP HTTP server
//#include <stdint.h>
//#include <string>
//#include <map>
// enum httpd_method_t{
//     HTTP_GET     = 0,
//     HTTP_POST    = 1,
//     HTTP_PUT     = 2,
//     HTTP_DELETE  = 3,
//     HTTP_PATCH   = 4,
//     HTTP_HEAD    = 5,
//     HTTP_OPTIONS = 6,
//     HTTP_TRACE   = 7,
//     HTTP_CONNECT = 8,
//     HTTP_ANY     = 9
// };

class serverManager{
    private:
        httpd_handle_t server = NULL;
        void _startRootFavicon();
    public:
        struct webData{
            const char* uri="";
            const char* html="";
            const char* status="";
            const char* error="";
        };
        struct collWData{
            httpd_method_t method=HTTP_GET;
            webData data;
        };
        struct apiOption{
            const char* rx="";// Expected request data
            const char* tx="";// Response data
            const char* type="application/json";// Response content type
            void (*handler)(void *pvPar)=nullptr;// Handler function
        };
        struct apiData{
            httpd_method_t method=HTTP_POST;
            const char* error="Content too long";
            const char* uri="";
            //esp_err_t (*handler)(char* buf,httpd_req_t *req)=nullptr;// Handler function
            std::map <std::string, apiOption> option;
        };
        std::map <std::string, collWData> collWeb;
        std::map <std::string, apiData> collApi;
        serverManager();
        httpd_handle_t begin();
        void addHTMLPath(std::string key,httpd_method_t method,webData data);
        void addAPIPath(std::string key, apiData apiData);
        void addStaticPath(std::string path, std::string filePath);
        static void sendResp(httpd_req_t *req,const char* resp,const char* type);
        // void removePath(std::string path);
        // void clearPaths();
        // void setPort(uint16_t port);// Set server port - default 80
        // void setMaxConnections(uint16_t maxConn);// Set maximum simultaneous connections - default 7
        // void setTimeout(uint32_t timeout_ms);// Set timeout in milliseconds - default 5000
        // void setStackSize(size_t stackSize);// Set stack size in bytes - default 4096
        // void setTaskPriority(UBaseType_t priority);// Set task priority - use tskIDLE_PRIORITY + n
        // void setCoreID(int coreID);// Set core ID for task affinity - use tskNO_AFFINITY for no affinity
        // void setTaskCapabilities(uint32_t capabilities);// Set task capabilities - bitwise OR of MALLOC_CAP_*
        void stop();
        ~serverManager();
};