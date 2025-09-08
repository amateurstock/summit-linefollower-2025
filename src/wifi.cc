#include "debug_tools.hh"

const char *ssid = "MSU GSC LF";
const char *pass = "12345678"; // someone change this later.

IPAddress local_ip (192, 168,  20,  25);
IPAddress gateway  (192, 168,  20,   1);
IPAddress subnet   (255, 255, 255,   0);

httpd_handle_t web_server = NULL;

esp_err_t init_wifi() {
    const char *TAG = "init_wifi";

    esp_err_t ret = ESP_OK;

    WiFi.persistent(false);

    if (!WiFi.mode(WIFI_AP)) {
        user_logger(TAG, "Failed to set WiFi mode!");
        ret = ESP_FAIL;
        return ret;
    }

    if (!WiFi.softAPConfig(local_ip, gateway, subnet)) {
        user_logger(TAG, "Failed to set AP config!");
        ret = ESP_FAIL;
        return ret;
    }

    if (!WiFi.softAP(ssid, pass)) {
        user_logger(TAG, "Failed to start access point!");
        ret = ESP_FAIL;
        return ret;
    }

    return ret;
}
