#include "vehicle.h"

int main(void) {
    vehicle_t vehicle;
    vehicle_init(&vehicle);

    for (;;) {
        switch (vehicle.state) {
            case STATE_GROUND:
                vehicle_update_ground(&vehicle);
                break;
            case STATE_FLIGHT:
                vehicle_update_flight(&vehicle);
                break;
        }
    }

    return 0;
}
