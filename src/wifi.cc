#include "debug_tools.hh"

const char *ssid = "MSU GSC LF";
const char *pass = "12345678"; // someone change this later.
const char *root_dir = "/data";
const char *html_path = "/index.html";
const char *js_path = "/scripts.js";

IPAddress local_ip (192, 168,  20,  25);
IPAddress gateway  (192, 168,  20,   1);
IPAddress subnet   (255, 255, 255,   0);

httpd_handle_t web_server = NULL;

// Wifi and Server Inits
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

esp_err_t start_server() {
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    cfg.max_uri_handlers = 16;
    const char *TAG = "start_server";
    user_logger(TAG, "Starting Server.");

    httpd_uri_t index_uri {
        .uri = "/",
        .method = HTTP_GET,
        .handler = index_handler,
        .user_ctx = NULL,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
    };

    Serial.printf("<%s> Starting web server on port: %d\n", TAG, cfg.server_port);
    if (httpd_start(&web_server, &cfg) == ESP_OK) {
        httpd_register_uri_handler(web_server, &index_uri);
    } else {
        user_logger(TAG, "Error! Web server won't start!");
        return ESP_FAIL;
    }

    user_logger(TAG, "Server has started!");
    return ESP_OK;
}


// Server Handlers and Utils
esp_err_t index_handler(httpd_req_t *req) {
    const char *TAG = "index_handler";

    return serve_files(req,
                       TAG,
                       html_path,
                       "text/html");
}

esp_err_t wifi_logger_handler(httpd_req_t *req) { // Not sure...
    const char *TAG = "wifi_logger_handler";
    httpd_resp_set_type(req, "text/plain");
    return httpd_resp_send(req, wifi_buf, strlen(wifi_buf));
}

esp_err_t params_handler(httpd_req_t *req) {
    const char *TAG = "params_handler";
    
    char *param_buf = NULL;
    char kp[32] = {0};
    char ki[32] = {0};
    char kd[32] = {0};
    char setpoint[32] = {0};
    char time_scale[32] = {0};

    if (parse_get(req, &param_buf) != ESP_OK) {
        user_logger(TAG, "Failed to parse request query.\n");
        return ESP_FAIL;
    }

    if (httpd_query_key_value(param_buf, "kp", kp, sizeof(kp)) != ESP_OK
        || httpd_query_key_value(param_buf, "ki", ki, sizeof(ki)) != ESP_OK
        || httpd_query_key_value(param_buf, "kd", kd, sizeof(kd)) != ESP_OK
        || httpd_query_key_value(param_buf, "setpoint", setpoint, sizeof(setpoint)) != ESP_OK
        || httpd_query_key_value(param_buf, "time_scale", time_scale, sizeof(time_scale)) != ESP_OK) {
        free(param_buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    free(param_buf);
    // Handle GET request values here.
}

esp_err_t serve_files(httpd_req_t *req, 
                      const char *TAG, 
                      const char *path,
                      const char *type) {
    String file_dir = "";
    file_dir += root_dir;
    file_dir += path;
    httpd_resp_set_type(req, type);

    Serial.printf("<%s> Attempting to open %s\n", TAG, file_dir.c_str());
    FILE *file = fopen(file_dir.c_str(), "r");
    if (!file) {
        Serial.printf("<%s> %s does not exist and/or is an invalid directory.\n",
                      TAG,
                      file_dir.c_str());
        httpd_resp_send_404(req);
        return ESP_ERR_NOT_FOUND;
    }

    user_logger(TAG, "Attempting to send file");
    char buf[256] = {0};
    while (fgets(buf, sizeof(buf) - 1, file)) {
        httpd_resp_send_chunk(req, buf, strlen(buf));
        memset(buf, '\0', sizeof(buf));
    }

    if (feof(file)) {
        httpd_resp_send_chunk(req, buf, 0);
        memset(buf, '\0', sizeof(buf));
    }

    fclose(file);
    user_logger(TAG, "File closed successfully!");
    return ESP_OK;
}

esp_err_t parse_get(httpd_req_t *req, char **obuf) {
    char *buf = NULL;
    size_t buf_len = 0;

    buf_len = httpd_req_get_url_query_len(req) + 1;

    if (buf_len > 1) {
        buf = (char *)malloc(buf_len);

        if (!buf) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            *obuf = buf;
            return ESP_OK;
        }
        free(buf);
    }
    httpd_resp_send_404(req);
    return ESP_FAIL;
}
