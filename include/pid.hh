#ifndef PID_HH
#define PID_HH
#include <Arduino.h>

class PID {
private:
    float _k_p = 0.0f;
    float _k_i = 0.0f;
    float _k_d = 0.0f;
    float _setpoint = 0.0f;
    uint32_t _last_time = 0;
    uint32_t _current_time = 0;
    uint32_t get_dt();
public:
    PID(
        float k_p,
        float k_i,
        float k_d,
        float setpoint
    );
    void initialize(uint32_t start_time);
    float calculate(uint32_t current_time);
};

#endif
