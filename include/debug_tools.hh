#ifndef DEBUG_TOOLS_HH
#define DEBUG_TOOLS_HH

#include "esp_spiffs.h"
#include <WiFi.h>
#include <esp_https_server.h>

// wifi.cc
esp_err_t init_wifi();

// print.cc
#define PRINT_BUFFER 256
#define WIFI_BUFFER 1024
extern char wifi_buf[WIFI_BUFFER];
void user_logger(const char *TAG, const char *message);
void wifi_logger(const char *message);
void wifi_logger_reinit();

#endif
