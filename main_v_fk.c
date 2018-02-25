#include "elev.h"
#include "elev_state_machine.h"
#include "utilities.h"
#include <stdio.h>

int main() {
    printf("Running main_v_fk");
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    printf("Press STOP button to stop elevator and exit program.\n");

    //Initializing elevator

    struct Elev elevator;
    elevator.status = INITIALIZE;
    while (1) {
        elevator = state_machine(elevator);
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }
    return 0;
}

