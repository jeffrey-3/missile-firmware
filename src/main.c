#include "vehicle.h"

int main(void) {
    vehicle_t vehicle;
    boot_mode_t boot_mode;

    vehicle_init(&vehicle);
    boot_mode = vehicle_run_cli(&vehicle);

    for (;;) {
        switch (boot_mode) {
            case BOOT_MODE_FLIGHT:
                vehicle_update_flight(&vehicle);
                break;
            case BOOT_MODE_CALIBRATE:
                vehicle_update_calibrate(&vehicle);
                break;
            case BOOT_MODE_RETREIVE:
                vehicle_update_retreive(&vehicle);
                break;
            case BOOT_MODE_ERASE:
                vehicle_update_erase(&vehicle);
                break;
        }
    }

    return 0;
}
