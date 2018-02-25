#include "elev.h"
#include "utilities.h"

bool is_at_order(int matrix[N_FLOORS][3], int current_floor){
  return(matrix[current_floor][0] == 1 || matrix[current_floor][1] == 1);
}

bool is_at_destiantion(int matrix[N_FLOORS][3], int current_floor){
  for(int i = 0; i < N_FLOORS; i++){
    if(matrix[i][2] == current_floor){
      return(true);
    }
  }
  return(false);
}

bool is_at_intermediate(int matrix[N_FLOORS][3], int current_floor, int current_dir){
    if(matrix[current_floor][get_index(current_dir)] == 1){
        for(int i = 0; i < N_FLOORS; i++){
            if((matrix[i][2] > current_floor && current_dir == DIRN_UP) ||
            (matrix[i][2] < current_floor && current_dir == DIRN_DOWN)){
                return(true);
            }
        }
    }
    return(false);
}