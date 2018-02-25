#include "elev.h"

void get_orders(int matrix[N_FLOORS][3]);
int check_ordered_destination();
void erase_order(int matrix[N_FLOORS][3], int current_floor);
void initialize_orders(int matrix[N_FLOORS][3]);
void reset_floor(int matrix[N_FLOORS][3], int current_floor);
void get_destination(int matrix[N_FLOORS][3], int current_floor);
int choose_direction(int matrix[N_FLOORS][3], int current_floor);