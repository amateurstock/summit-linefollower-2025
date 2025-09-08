#ifndef PID_HH
#define PID_HH

#include <Arduino.h>

class PID {
private:
    float _last_error = 0.0f;
    float _error_total = 0.0f;
    float _time_scale = 1.0f;
    uint32_t _last_time = 0;
    uint32_t _current_time = 0;

    uint32_t get_dt(uint32_t current_time);

public:
    float _k_p = 0.0f;
    float _k_i = 0.0f;
    float _k_d = 0.0f;
    float _setpoint = 0.0f;

    PID(float k_p,
        float k_i,
        float k_d,
        float setpoint,
        float time_scale);
    float calculate(uint32_t current_time, float signal);
    void initialize(uint32_t current_time);
    void update_setpoint(float new_setpoint);
    void update_constants(float k_p,
                          float k_i,
                          float k_d);
};

#endif
