#include "../../src/seeker.h"
#include "sim_interface.h"

void seeker_init(seeker_t *seeker) {
    (void)seeker;
}
void seeker_update(seeker_t * seeker) {
    seeker->pitch_error = sim_receive.pitch_error;
    seeker->yaw_error = sim_receive.yaw_error;
}
