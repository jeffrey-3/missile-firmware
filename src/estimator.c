#include "estimator.h"

void estimator_init(estimator_t *estimator, spi_t *imu_spi) {
    estimator->imu_spi = imu_spi;
    estimator->q = quat_identity();
    estimator->vel.x = 0.0f;
    estimator->vel.y = 0.0f;
    estimator->vel.z = 0.0f;
    estimator->pos.x = 0.0f;
    estimator->pos.y = 0.0f;
    estimator->pos.z = 0.0f;
    estimator->acc_sum.x = 0.0f;
    estimator->acc_sum.y = 0.0f;
    estimator->acc_sum.z = 0.0f;
    estimator->timer = 0;
    estimator->state = ESTIMATOR_STATE_ALIGN;

    icm45686_init(&estimator->imu, estimator->imu_spi);
}

void estimator_update(estimator_t *estimator) {
    if (!timer_expired(&estimator->timer, 10)) return;

    float dt = 0.01f;

    icm45686_read_accel(&estimator->imu, estimator->accel);
    icm45686_read_gyro(&estimator->imu, estimator->gyro);

    estimator->gyro[0] -= GYRO_OFF_X;
    estimator->gyro[1] -= GYRO_OFF_Y;
    estimator->gyro[2] -= GYRO_OFF_Z;

    switch (estimator->state) {
        case ESTIMATOR_STATE_ALIGN:
            estimator_align_update(estimator);
            break;
        case ESTIMATOR_STATE_RUNNING:
            estimator_attitude_update(estimator, dt);
            estimator_position_update(estimator, dt);
            break;
    }
}

void estimator_align_update(estimator_t *estimator) {
    estimator->acc_sum.x += estimator->accel[0];
    estimator->acc_sum.y += estimator->accel[1];
    estimator->acc_sum.z += estimator->accel[2];
    estimator->acc_count++;

    if (estimator->acc_count > ESTIMATOR_ALIGN_SAMPLES) {
        float ax_avg = estimator->acc_sum.x / (float)estimator->acc_count;
        float ay_avg = estimator->acc_sum.y / (float)estimator->acc_count;
        float az_avg = estimator->acc_sum.z / (float)estimator->acc_count;

        float roll = atan2f(-ay_avg, -az_avg);
        float pitch = atan2f(ax_avg, sqrtf(ay_avg*ay_avg + az_avg*az_avg));

        estimator->q = quat_from_euler(roll, pitch, 0.0f);
        estimator->state = ESTIMATOR_STATE_RUNNING;
    }
}

void estimator_attitude_update(estimator_t *estimator, float dt) {
    quat_t dq;
    dq.w = 1.0f;
    dq.x = 0.5f * estimator->gyro[0] * dt;
    dq.y = 0.5f * estimator->gyro[1] * dt;
    dq.z = 0.5f * estimator->gyro[2] * dt;

    estimator->q = quat_mul(estimator->q, dq);
    quat_normalize(&estimator->q);
}

void estimator_position_update(estimator_t *estimator, float dt) {
    vec3_t acc_body = {estimator->accel[0] * 9.81f, estimator->accel[1] * 9.81f,
        estimator->accel[2] * 9.81f};
    estimator->acc_world = quat_rotate_vector(estimator->q, acc_body);
    estimator->acc_world.z += 9.81f;

    estimator->vel.x += estimator->acc_world.x * dt;
    estimator->vel.y += estimator->acc_world.y * dt;
    estimator->vel.z += estimator->acc_world.z * dt;

    estimator->pos.x += estimator->vel.x * dt;
    estimator->pos.y += estimator->vel.y * dt;
    estimator->pos.z += estimator->vel.z * dt;
}
