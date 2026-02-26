#include "../../src/vehicle.h"
#include "sim_interface.h"

int main(void) {
    vehicle_t vehicle;
    vehicle_init(&vehicle);

    sim_interface_init();

    for (;;) {
        vehicle_update(&vehicle);
    }

    return 0;
}
