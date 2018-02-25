#include "math.h"
#include "elev.h"
#include <stdio.h>
#include "timer.h"

void start_timer(){
	time(&start_value); // time() returns seconds since a pre-defined happening
}

int check_timer(double seconds){        //Returns 1 if 'seconds' seconds have past
	time(&end_value);
	if (difftime(end_value, start_value) >= seconds){
		return 1;
	
	}
	return 0;
}


int get_index(int current_dir){         //Returns index (0 or 1) depending on current direction of elevator
  int index = 1;
  if(current_dir == 1){
    index = 0;
  }
  return(index);
}

int get_sign(int val){                 //Returns sign(val)
  if(val == 0){
    return(val);
  }
  return(val/abs(val));
}

void print_matrix(int matrix[N_FLOORS][3]){
  for(int i = 0; i < N_FLOORS; i++){
    for(int j = 0; j < 3; j++){
      printf("%d, ", matrix[i][j]);
    }
    printf("\n");
  }
}

int set_direction(int current_floor, int destination){
    if(destination == -1){
        return(0);
    }
    return(get_sign(destination - current_floor));
}