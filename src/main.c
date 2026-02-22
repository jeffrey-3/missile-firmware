#include "vehicle.h"

int main(void) {
    vehicle_t vehicle;
    boot_mode_t boot_mode;

    vehicle_init(&vehicle);
    boot_mode = vehicle_run_cli(&vehicle);

    switch (boot_mode) {
        case BOOT_MODE_FLIGHT:
            for (;;) {
                vehicle_update_flight(&vehicle);
            }
            break;
        case BOOT_MODE_CALIBRATE:
            for (;;) {
                vehicle_update_calibrate(&vehicle);
            }
            break;
        case BOOT_MODE_RETRIEVE:
            vehicle_retrieve(&vehicle);
            break;
        case BOOT_MODE_ERASE:
            vehicle_erase(&vehicle);
            break;
    }

    return 0;
}
