#include "pid.hh"

/*
 * @desc: PID constructor. Sets the object's params to the specified args.
 * @param k_p: Constant for proprotional.
 * @param k_i: Constant for integral.
 * @param k_d: Constant for differential.
 * @param setpoint: Target value for PID to achieve
 * @param time_scale: Tweaks effect of time-sensitive calculations.
 */
PID::PID(float k_p,
         float k_i,
         float k_d,
         float setpoint,
         float time_scale) {
    _k_p = k_p;
    _k_i = k_i;
    _k_d = k_d;
    _setpoint = setpoint;
    _time_scale = time_scale;
}

uint32_t PID::get_dt(uint32_t current_time) {
    _last_time = _current_time;
    _current_time = current_time;
    return (_current_time - _last_time);
}

void PID::initialize(uint32_t current_time) {
    _current_time = current_time;
}

/*
 * @desc: Instructs the object to calculate the resulting PID feedback value.
 * @param current_time: Timestamp @ calculation time. Recommended: use millis().
 * @param signal: Feedback signal coming from sensors. Must be in float value.
 *                Otherwise, cast using (float).
 */
float PID::calculate(uint32_t current_time, float signal) {
    float error = _setpoint - signal;
    float ret = 0.0f;
    float dt = (float) get_dt(current_time);

    // Proportional Section
    ret += _k_p * error;

    // Integral Section
    _error_total += error * dt * _time_scale;
    ret += _k_i * _error_total;

    // Differential Section
    ret += _k_d * (error - _last_error) / (dt * _time_scale);
    _last_error = error;

    return ret;
}

/*
 * @desc: Change PID object's setpoint value.
 * @param new_setpoint: A new setpoint value.
 */
void PID::update_setpoint(float new_setpoint) {
    _setpoint = new_setpoint;
}

/*
 * @desc: Update's PID object's PID constant values.
 * @param k_p: A new proportional constant value.
 * @param k_i: A new integral constant value.
 * @param k_d: A new differential constant value.
 */
void PID::update_constants(float k_p,
                           float k_i,
                           float k_d) {
    _k_p = k_p;
    _k_i = k_i;
    _k_d = k_d;
};
