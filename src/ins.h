#ifndef INS_H
#define INS_H

#include <stdint.h>
#include <stdbool.h>
#include "hal/spi.h"
#include "peripherals/icm45686.h"
#include "math/quaternion.h"
#include "config.h"

#define INS_ALIGN_SAMPLES 100

typedef enum {
    INS_STATE_ALIGN,
    INS_STATE_RUNNING
} ins_state_t;

typedef struct {
    spi_t *imu_spi;
    icm45686_t imu;
    ins_state_t state;
    quat_t q;
    vec3_t pos;
    vec3_t vel;
    vec3_t acc_world;
    vec3_t acc_sum;
    uint16_t acc_count;
    float accel[3];
    float gyro[3];
} ins_t;

void ins_init(ins_t *ins, spi_t *imu_spi);
void ins_update(ins_t *ins, float dt);
void ins_align_update(ins_t *ins);
void ins_attitude_update(ins_t *ins, float dt);
void ins_position_update(ins_t *ins, float dt);

#endif // INS_H
