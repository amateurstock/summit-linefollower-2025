#ifndef DEBUG_TOOLS_HH
#define DEBUG_TOOLS_HH

#include "esp_spiffs.h"
#include <WiFi.h>
#include <esp_https_server.h>

// wifi.cc
esp_err_t init_wifi();
esp_err_t index_handler(httpd_req_t *req);
esp_err_t serve_files(httpd_req_t *req, 
                      const char *TAG, 
                      const char *path,
                      const char *type);
esp_err_t parse_get(httpd_req_t *req, char **obuf);

// print.cc
typedef enum {
    STRING,
    INTEGER,
    FLOAT
} log_type_t;

typedef struct {
    log_type_t type;
    void *content;
} wifi_log_t;

constexpr uint32_t PRINT_BUFFER = 256;
constexpr uint32_t WIFI_BUFFER = 1024;
extern char wifi_buf[WIFI_BUFFER];
void user_logger(const char *TAG, const char *message);
void wifi_logger(const char *message);
void wifi_logger_reinit();

#endif
