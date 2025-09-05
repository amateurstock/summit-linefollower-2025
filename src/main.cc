#include "tasks.hh"

void setup() {
    Serial.begin(115200);
    ESP_ERROR_CHECK(init_tasks());
}

void loop() {
    Serial.println("Hello world!");
    vTaskDelay(pdMS_TO_TICKS(1000));
}
