#include "debug_tools.hh"

char buf[PRINT_BUFFER] = {0};
char wifi_buf[WIFI_BUFFER] = {0};

void user_logger(const char *TAG, const char *message) {
    buf[0] = '\0';
    snprintf(buf, sizeof(buf), "%s %s\n", TAG, message);
    Serial.print(buf);
}

void wifi_logger(const char *message) {
}

// I have no idea if this is the right way to do this.
void wifi_logger_reinit() {
    wifi_buf[0] = '\0';
}

