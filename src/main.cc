#include "tasks.hh"
#include "debug_tools.hh"

void setup() {
    Serial.begin(115200);
    ESP_ERROR_CHECK(init_tasks());

#ifdef WIFI_DBG
    ESP_ERROR_CHECK(init_wifi());
#endif

}

void loop() {
    Serial.println("Hello world!");
    vTaskDelay(pdMS_TO_TICKS(1000));
}
