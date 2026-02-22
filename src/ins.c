#include "ins.h"

void ins_init(ins_t *ins, spi_t *imu_spi) {
    ins->imu_spi = imu_spi;
    ins->q = quat_identity();
    ins->vel.x = 0.0f;
    ins->vel.y = 0.0f;
    ins->vel.z = 0.0f;
    ins->pos.x = 0.0f;
    ins->pos.y = 0.0f;
    ins->pos.z = 0.0f;
    ins->acc_sum.x = 0.0f;
    ins->acc_sum.y = 0.0f;
    ins->acc_sum.z = 0.0f;
    ins->state = INS_STATE_ALIGN;

    icm45686_init(&ins->imu, ins->imu_spi);
}

void ins_update(ins_t *ins, float dt) {
    icm45686_read_accel(&ins->imu, ins->accel);
    icm45686_read_gyro(&ins->imu, ins->gyro);

    ins->gyro[0] -= GYRO_OFF_X;
    ins->gyro[1] -= GYRO_OFF_Y;
    ins->gyro[2] -= GYRO_OFF_Z;

    switch (ins->state) {
        case INS_STATE_ALIGN:
            ins_align_update(ins);
            break;
        case INS_STATE_RUNNING:
            ins_attitude_update(ins, dt);
            ins_position_update(ins, dt);
            break;
    }
}

void ins_align_update(ins_t *ins) {
    ins->acc_sum.x += ins->accel[0];
    ins->acc_sum.y += ins->accel[1];
    ins->acc_sum.z += ins->accel[2];
    ins->acc_count++;

    if (ins->acc_count > INS_ALIGN_SAMPLES) {
        float ax_avg = ins->acc_sum.x / (float)ins->acc_count;
        float ay_avg = ins->acc_sum.y / (float)ins->acc_count;
        float az_avg = ins->acc_sum.z / (float)ins->acc_count;

        float roll = atan2f(-ay_avg, -az_avg);
        float pitch = atan2f(ax_avg, sqrtf(ay_avg*ay_avg + az_avg*az_avg));

        ins->q = quat_from_euler(roll, pitch, 0.0f);
        ins->state = INS_STATE_RUNNING;
    }
}

void ins_attitude_update(ins_t *ins, float dt) {
    quat_t dq;
    dq.w = 1.0f;
    dq.x = 0.5f * ins->gyro[0] * dt;
    dq.y = 0.5f * ins->gyro[1] * dt;
    dq.z = 0.5f * ins->gyro[2] * dt;

    ins->q = quat_mul(ins->q, dq);
    quat_normalize(&ins->q);
}

void ins_position_update(ins_t *ins, float dt) {
    vec3_t acc_body = {ins->accel[0] * 9.81f, ins->accel[1] * 9.81f,
        ins->accel[2] * 9.81f};
    ins->acc_world = quat_rotate_vector(ins->q, acc_body);
    ins->acc_world.z += 9.81f;

    ins->vel.x += ins->acc_world.x * dt;
    ins->vel.y += ins->acc_world.y * dt;
    ins->vel.z += ins->acc_world.z * dt;

    ins->pos.x += ins->vel.x * dt;
    ins->pos.y += ins->vel.y * dt;
    ins->pos.z += ins->vel.z * dt;
}
