#include "debug_tools.hh"

char serial_buf[WIFI_BUF_SIZE] = {'\0'};

Log::Log() {
    _type = EMPTY;
    _TAG = "";
    _content._int_val = 0;
}
Log::Log(const char *TAG, const char *str_val) {
    _type = STRING;
    _TAG = TAG;
    _content._str_val = str_val;
}
Log::Log(const char *TAG, int32_t int_val) {
    _type = INT;
    _TAG = TAG;
    _content._int_val = int_val;
}
Log::Log(const char *TAG, uint32_t uint_val) {
    _type = UINT;
    _TAG = TAG;
    _content._uint_val = uint_val;
}
Log::Log(const char *TAG, float flt_val) {
    _type = FLT;
    _TAG = TAG;
    _content._flt_val = flt_val;
}

WifiLogger::WifiLogger() {
    Log empty;
    for (uint32_t i = 0; i < QUEUE_SIZE; i += 1) {
        _queue[i] = empty;
    }
    _is_ready = true;
}

void WifiLogger::log(Log log) {
    while (!_is_ready) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    _is_ready = false;
    for (uint32_t i = 0; i < QUEUE_SIZE; i += 1) {
        if (strcmp(log._TAG, _queue[i]._TAG) == 0) {
            _queue[i] = log;
            _is_ready = true;
            return;
        }
        if (_queue[i]._type != EMPTY) {
            continue;
        }
        _queue[i] = log;
        _is_ready = true;
        return;
    }
}

void WifiLogger::print() {
    while (!_is_ready) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    _is_ready = false;
    char *wifi_ptr = _wifi_buf;
    uint32_t total_len = 0;
    for (uint32_t i = 0; i < QUEUE_SIZE; i += 1) {
        switch(_queue[i]._type) {
            case STRING: {
                total_len += snprintf(wifi_ptr,
                                      sizeof(_wifi_buf) - total_len,
                                      "<%s>%s;",
                                      _queue[i]._TAG,
                                      _queue[i]._content._str_val);
                wifi_ptr = _wifi_buf + total_len;
                break;
            }
            case INT: {
                total_len += snprintf(wifi_ptr,
                                      sizeof(_wifi_buf) - total_len,
                                      "<%s>%" PRIi32 ";",
                                      _queue[i]._TAG,
                                      _queue[i]._content._int_val);
                wifi_ptr = _wifi_buf + total_len;
                break;
            }
            case UINT: {
                total_len += snprintf(wifi_ptr,
                                      sizeof(_wifi_buf) - total_len,
                                      "<%s>%" PRIu32 ";",
                                      _queue[i]._TAG,
                                      _queue[i]._content._uint_val);
                wifi_ptr = _wifi_buf + total_len;
                break;
            }
            case FLT: {
                total_len += snprintf(wifi_ptr,
                                      sizeof(_wifi_buf) - total_len,
                                      "<%s>%.3f;",
                                      _queue[i]._TAG,
                                      _queue[i]._content._flt_val);
                wifi_ptr = _wifi_buf + total_len;
                break;
            }
            case EMPTY: {
                break;
            }
        }
    }
    _buf_len = total_len;
    _is_ready = true;
}

void WifiLogger::flush() {
    if (!_is_ready) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    _is_ready = false;

    Log empty;
    for (uint32_t i = 0; i < QUEUE_SIZE; i += 1) {
        _queue[i] = empty;
    }

    _is_ready = true;
}

void user_logger(const char *TAG, const char *message) {
    snprintf(serial_buf,
             sizeof(serial_buf),
             "<%s>:%s\n",
             TAG,
             message);
    Serial.print(serial_buf);
    serial_buf[0] = '\0';
}
void user_logger(const char *TAG, uint32_t u32) {
    snprintf(serial_buf,
             sizeof(serial_buf),
             "<%s>:%" PRIu32 "\n",
             TAG,
             u32);
    Serial.print(serial_buf);
    serial_buf[0] = '\0';
}
void user_logger(const char *TAG, int32_t i32) {
    snprintf(serial_buf,
             sizeof(serial_buf),
             "<%s>:%" PRIi32 "\n",
             TAG,
             i32);
    Serial.print(serial_buf);
    serial_buf[0] = '\0';
}
void user_logger(const char *TAG, float flt) {
    snprintf(serial_buf,
             sizeof(serial_buf),
             "<%s>:%f\n",
             TAG,
             flt);
    Serial.print(serial_buf);
    serial_buf[0] = '\0';
}
