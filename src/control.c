#include "control.h"

void control_init(control_t *control, pwm_t *servo_y, pwm_t *servo_z) {
    control->servo_y = servo_y;
    control->servo_z = servo_z;
    control->max_pulse = 2100;
    control->min_pulse = 900;
    control->p_gain = 0.2f;
    control->control_timer = 0;
}

void control_update(control_t *control, ins_t *ins) {
    if (!timer_expired(&control->control_timer, 20)) return;

    float pitch_setpoint = 0 * DEG2RAD;
    float yaw_setpoint = 0 * DEG2RAD;

    quat_to_euler(ins->q, &control->roll, &control->pitch, &control->yaw);

    float pitch_error = pitch_setpoint - control->pitch;
    float yaw_error = yaw_setpoint - control->yaw;

    float servo_y_angle = control->p_gain * pitch_error; // Radians
    float servo_z_angle = control->p_gain * yaw_error;

    float center_pulse = ((float)control->max_pulse + (float)control->min_pulse)
        / 2.0f;
    uint16_t servo_y_pulse = (uint16_t)(center_pulse +
        ((float)control->max_pulse - (float)control->min_pulse) *
        servo_y_angle / (15.0f * DEG2RAD));
    uint16_t servo_z_pulse = (uint16_t)(center_pulse +
        ((float)control->max_pulse - (float)control->min_pulse) *
        servo_z_angle / (15.0f * DEG2RAD));

    servo_y_pulse = clamp_u16(servo_y_pulse, control->min_pulse,
        control->max_pulse);
    servo_z_pulse = clamp_u16(servo_z_pulse, control->min_pulse,
        control->max_pulse);

    pwm_set_pulse(control->servo_y, servo_y_pulse);
    pwm_set_pulse(control->servo_z, servo_z_pulse);
}
