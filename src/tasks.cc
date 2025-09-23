#include "tasks.hh"
#include "pid.hh"
#include "debug_tools.hh"
#include "renan_base.hh"

WifiLogger wifi_logger;

esp_err_t init_tasks() {
    const char *TAG = "init_tasks";
    user_logger(TAG, "Attempting to init tasks.");
    esp_err_t ret = ESP_OK;

    user_logger(renan_loop_tag, "Initializing renan_loop.");
    xTaskCreate(
        renan_loop,
        "renan_loop task",
        4096,
        NULL,
        5,
        &renan_loop_t
    );
    if (renan_loop_t == NULL) {
        user_logger(renan_loop_tag, "Failed to create task...");
        ret = ESP_FAIL;
    }
    return ret;
}
