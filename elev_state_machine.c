#include "checks.h"
#include "order_system.h"
#include "elev.h"
#include "elev_state_machine.h"


struct Elev state_machine(struct Elev elevator){
    switch(elevator.status){
        
        case INITIALIZE:    //Moving elevator to nearest floor above start position
            initialize_orders(elevator.orders);                           
            elevator.stopped_at_intermediate = false;           
            elev_set_motor_direction(DIRN_UP);            
            elevator.current_floor = start();                             
            elevator.current_dir = DIRN_STOP;
            elevator.status = IDLE;
            break;
        
        case IDLE:
            get_orders(elevator.orders);
            get_destination(elevator.orders, elevator.current_floor);
            elevator.status = EXCECUTE;
            break;
        
        case EXCECUTE:       
                elevator.current_dir = choose_direction(elevator.orders, elevator.current_floor);
                if(elevator.current_dir != 0){
                    elevator.status = IN_TRANSIT;
                }
                else{
                    elevator.status = IDLE;
                }
                break;
        
        case IN_TRANSIT:
            get_orders(elevator.orders);
            elevator.current_floor = elev_get_floor_sensor_signal();
            if(elevator.current_floor != -1){
                elevator.status = RUN_TESTS;
            }
            else{
                elevator.status = IN_TRANSIT;
            }
            break;
        
        case RUN_TESTS:
             if(is_at_intermediate(elevator.orders, elevator.current_floor, elevator.current_dir)){
                elevator.stopped_at_intermediate = true;
                elevator.status = AT_ORDER;
                break;
            }
            if(is_at_order(elevator.orders, elevator.current_floor)){
                elevator.status = AT_ORDER;
                break;
            }
            else if(is_at_destiantion(elevator.orders, elevator.current_floor)){
                elevator.status = AT_DESTINATION;
                break;
            }
            elevator.status = IN_TRANSIT;
            break;
        
        case AT_ORDER:
            reset_floor(elevator.orders, elevator.current_floor);
            elev_set_motor_direction(DIRN_STOP);
            start_timer();
            while(!check_timer(3)){
                get_orders(elevator.orders);
                get_destination(elevator.orders, elevator.current_floor);
            }
            if(!elevator.stopped_at_intermediate){
                 elevator.current_dir = set_direction(elevator.current_floor, elevator.orders[elevator.current_floor][2]);
            }
            if(elevator.current_dir == DIRN_STOP){
                elevator.status = IDLE;
            }
            else{
                elevator.status = IN_TRANSIT;
            }
            break;

        case AT_DESTINATION:
            erase_order(elevator.orders, elevator.current_floor);
            elev_set_motor_direction(DIRN_STOP);
            start_timer();
            while(!check_timer(3)){
                get_orders(elevator.orders);
                get_destination(elevator.orders, elevator.current_floor);
            }
            if(elevator.stopped_at_intermediate){
                elevator.status = IN_TRANSIT;
            }
            else{
                elevator.current_dir = DIRN_STOP;
                elevator.status = EXCECUTE;
            }
        default:
            break;

    elev_set_motor_direction(elevator.current_dir);
    }
}