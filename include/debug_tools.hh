#ifndef DEBUG_TOOLS_HH
#define DEBUG_TOOLS_HH
#define WIFI_DBG

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
constexpr uint32_t QUEUE_SIZE = 16;
constexpr uint32_t WIFI_BUF_SIZE = 256;

typedef enum {
    STRING,
    INT,
    UINT,
    FLT,
    EMPTY
} log_type_t;

class Log {
public:
    log_type_t _type;
    const char *_TAG;
    union {
        const char *_str_val;
        int32_t _int_val;
        uint32_t _uint_val;
        float _flt_val;
    } _content;

    Log(const char *TAG, const char *str_val);
    Log(const char *TAG, int32_t int_val);
    Log(const char *TAG, uint32_t uint_val);
    Log(const char *TAG, float flt_val);
    Log();
};

class WifiLogger {
private:
    bool _is_ready;
    Log _queue[QUEUE_SIZE];
public:
    char _wifi_buf[WIFI_BUF_SIZE];
    uint32_t _buf_len;

    WifiLogger();
    void log(Log log);
    void print();
    void flush();
};

void user_logger(const char *TAG, const char *message);
void user_logger(const char *TAG, uint32_t u32);
void user_logger(const char *TAG, int32_t i32);
void user_logger(const char *TAG, float flt);

#endif
