#include "elev.h"
#include "elev_state_machine.h"
#include "utilities.h"
#include <stdio.h>

int start(){
  while(1){
    if(elev_get_floor_sensor_signal() != -1){  //Run until elevator reaches a floor
      elev_set_motor_direction(DIRN_STOP);     //Stop elevator when it hits a floor
      return(elev_get_floor_sensor_signal());  //Return which floor elevator stopped at
    }
  }
}


int main() {
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

