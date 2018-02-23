#include "elev.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

void print_matrix(int matrix[N_FLOORS][3]){
  for(int i = 0; i < N_FLOORS; i++){
    for(int j = 0; j < 3; j++){
      printf("%d, ", matrix[i][j]);
    }
    printf("\n");
  }
}

int get_index(int current_dir){
  int index = 1;
  if(current_dir == 1){
    index = 0;
  }
  return(index);
}


int start(int* current_dir){
  while(1){
    if(elev_get_floor_sensor_signal() != -1){
      elev_set_motor_direction(DIRN_STOP);
      *current_dir = 0;
      return(elev_get_floor_sensor_signal());
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

int get_sign(int val){
  return(val/abs(val));
}

void go_to_order(int matrix[N_FLOORS][3], int* current_dir, int* current_floor){
  int min_distance = N_FLOORS;
  int min_index = 0;
  for(int i = 0; i < N_FLOORS; i++){
    if((matrix[i][0] != 0 || matrix[i][1] != 0) && abs(i - *current_floor) < abs(min_distance) && abs(i - *current_floor) != 0){
      matrix[min_index][2] = -1;
      min_distance = i - *current_floor;
      matrix[*current_floor + min_distance][2] = i;
      min_index = i;
      *current_dir = get_sign(min_distance);
    }
  elev_set_motor_direction(*current_dir);
  }
}

bool is_at_order(int matrix[N_FLOORS][3], int current_floor){
  for(int i = 0; i < N_FLOORS; i++){
    if(current_floor == matrix[i][2] && i == matrix[i][2]){
      return(true);
    }
  }
  return(false);
}

bool is_at_destiantion(int matrix[N_FLOORS][3], int current_floor){
  for(int i = 0; i < N_FLOORS; i++){
    if(current_floor == matrix[i][2] && i != matrix[i][2]){
      return(true);
    }
  }
  return(false);
}

bool is_at_intermediate(int matrix[N_FLOORS][3], int current_floor, int current_dir){
  for(int i = 0; i < N_FLOORS; i++){
    if(current_floor != matrix[i][2] && matrix[i][get_index(current_dir)] == 1 && i == current_floor){
      return(true);
    }
  }
  return(false);
}

int check_ordered_destination(){
  for(int i = 0; i < N_FLOORS; i++){
    if(elev_get_button_signal(2, i)){
      return(i);
    }
  }
  return(-1);
}

void erase_order(int matrix[N_FLOORS][3], int current_floor){
  for(int i = 0; i < N_FLOORS; i++){
    if(matrix[i][2] == current_floor){
      matrix[i][2] = -1;
    }
  }
}

void reset_floor(int matrix[N_FLOORS][3], int current_floor){
  matrix[current_floor][0] = 0, matrix[current_floor][1] = 0, matrix[current_floor][2] = -1;
}

int set_destination(int matrix[N_FLOORS][3], int current_floor){
  while(matrix[current_floor][2] == -1){
    matrix[current_floor][2] = check_ordered_destination();
  }
  return(get_sign(matrix[current_floor][2] - current_floor));
}

int main() {
    // Initialize hardware
    int matrix[N_FLOORS][3] = {{0,0,-1},
                        {0,0,-1},
                        {0,0,-1},
                        {0,0,-1}};
    
    int current_floor_val = 69;
    int* current_floor = &current_floor_val;
    int current_dir_val = 1;
    int* current_dir = &current_dir_val;

    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);
    *current_dir = 1;
    *current_floor = start(current_dir);

    while (1) {
      *current_floor = elev_get_floor_sensor_signal();
      get_orders(matrix); 
      if(*current_dir == 0){
          go_to_order(matrix, current_dir, current_floor);
        }
      if (*current_dir!=0 && *current_floor != -1){
        bool at_destination = is_at_destiantion(matrix, *current_floor);
        bool at_intermediate = is_at_intermediate(matrix, *current_floor, *current_dir);
        bool at_order = is_at_order(matrix, *current_floor);
        if(at_destination){
          printf("-----------DESTINATION---------");
          print_matrix(matrix);
          *current_dir = 0;
          matrix[*current_floor][2] = -1;
          erase_order(matrix, *current_floor);
          printf("----------EDITED----------------");
          print_matrix(matrix);
        }
        else if(at_order){
          printf("-----------ORDER---------");
          print_matrix(matrix);
          *current_dir = 0;
          elev_set_motor_direction(*current_dir);
          reset_floor(matrix, *current_floor);
          *current_dir = set_destination(matrix, *current_floor);
          printf("----------EDITED----------------");
          print_matrix(matrix);
        }
        else if(at_intermediate){
          printf("-----------INTERMEDIATE---------");
          print_matrix(matrix);
          elev_set_motor_direction(0);
          reset_floor(matrix, *current_floor);
          set_destination(matrix, *current_floor);
          printf("----------EDITED----------------");
          print_matrix(matrix);
        }
        elev_set_motor_direction(*current_dir);
      }
      if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
      }
    return 0;
}
