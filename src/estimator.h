#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "hal/spi.h"
#include "peripherals/icm45686.h"
#include "math/quaternion.h"

#define ESTIMATOR_ALIGN_SAMPLES 100
#define GYRO_OFF_X 0.00795f
#define GYRO_OFF_Y -0.00295f
#define GYRO_OFF_Z 0.0002f

typedef enum {
    ESTIMATOR_STATE_ALIGN,
    ESTIMATOR_STATE_RUNNING
} estimator_state_t;

typedef struct {
    spi_t *imu_spi;
    icm45686_t imu;
    estimator_state_t state;
    quat_t q;
    vec3_t pos;
    vec3_t vel;
    vec3_t acc_world;
    vec3_t acc_sum;
    uint16_t acc_count;
    float accel[3];
    float gyro[3];
    uint32_t timer;
} estimator_t;

void estimator_init(estimator_t *estimator, spi_t *imu_spi);
void estimator_update(estimator_t *estimator);
void estimator_align_update(estimator_t *estimator);
void estimator_attitude_update(estimator_t *estimator, float dt);
void estimator_position_update(estimator_t *estimator, float dt);

#endif // ESTIMATOR_H
