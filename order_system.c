#include "elev.h"
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"


int start(){
  while(1){
    if(elev_get_floor_sensor_signal() != -1){  //Run until elevator reaches a floor
      elev_set_motor_direction(DIRN_STOP);     //Stop elevator when it hits a floor
      return(elev_get_floor_sensor_signal());  //Return which floor elevator stopped at
    }
  }
}

void get_orders(int matrix[N_FLOORS][3]){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < N_FLOORS; j++){
            if(!((i == 0 && j == 3) || (i == 1 && j == 0))){ //Sjekker opp/ned i alle
                if(matrix[j][i] == 0){                         //etg bortsett fra ned i etg. 1 og opp i etg. N_FLOORS
                    matrix[j][i] = elev_get_button_signal(i, j);
                }
            }
        }
    }
}

int check_ordered_destination(){
    for(int i = 0; i < N_FLOORS; i++){
        if(elev_get_button_signal(BUTTON_COMMAND, i)){
            printf("%d\n", i);
            return(i);
        }
    }
    return(-1);
}

void get_destination(int matrix[N_FLOORS][3], int current_floor){
    matrix[current_floor][2] = check_ordered_destination();
}

void erase_order(int matrix[N_FLOORS][3], int current_floor){
    for(int i = 0; i < N_FLOORS; i++){
        if(matrix[i][2] == current_floor){
        matrix[i][2] = -1;
    }
  }
}

void initialize_orders(int matrix[N_FLOORS][3]){
    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < 3; j++){
            matrix[i][j] = 0;
            if(j == 2){
                matrix[i][j] = -1;
            }
        }
    }
}

void reset_floor(int matrix[N_FLOORS][3], int current_floor){
  matrix[current_floor][0] = 0, matrix[current_floor][1] = 0, matrix[current_floor][2] = -1;
}

int choose_direction(int matrix[N_FLOORS][3], int current_floor){
    int min_distance = N_FLOORS, dir = 0;
    for(int i = 0; i < N_FLOORS; i++){  //Look for orders at each floor
        if((matrix[i][0] != 0 || matrix[i][1] != 0) && abs(i - current_floor) < abs(min_distance) && abs(i - current_floor) != 0){
            min_distance = i - current_floor;
            dir = get_sign(min_distance);
            printf("DIR = %d\n", dir);
        }
    }        
    if(dir == 0){   //If no orders has been made, look for destination
        dir = set_direction(current_floor, matrix[current_floor][2]);
    }
    return(dir);    
}

void monitor_buttons(int matrix[N_FLOORS][3], int current_floor){
    get_orders(matrix);
    if(matrix[current_floor][2] == -1){
        get_destination(matrix, current_floor);
    }
}