#include "vehicle.h"

int main(void) {
    vehicle_t vehicle;

    vehicle_init(&vehicle);

    for (;;) {
        vehicle_update(&vehicle);
    }

    return 0;
}
