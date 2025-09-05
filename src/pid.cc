#include "pid.hh"

PID::PID(
    float k_p,
    float k_i,
    float k_d,
    float setpoint
) {
    _k_p = k_p;
    _k_i = k_i;
    _k_d = k_d;
    _setpoint = setpoint;
}

void PID::initialize(uint32_t start_time) {
    _last_time = start_time;
}

uint32_t get_dt() {
}

float PID::calculate(uint32_t current_time) {
}
