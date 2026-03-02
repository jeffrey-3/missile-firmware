#include "../../src/vehicle.h"
#include "sim_interface.h"

int main(void) {
    vehicle_t vehicle;
    sim_interface_t sim;

    vehicle_init(&vehicle);
    sim_interface_init(&sim);

    for (;;) {
        // vehicle_update(&vehicle);
        sim_interface_update(&sim);
    }

    return 0;
}
